/*
The zlib/libpng License
http://opensource.org/licenses/zlib-license.php


Angelscript addon Template Containers
Copyright (c) 2014 Sami Vuorela

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1.	The origin of this software must not be misrepresented;
You must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.

2.	Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3.	This notice may not be removed or altered from any source distribution.


Sami Vuorela
samivuorela@gmail.com
*/



#include "aatc_common.hpp"

//#include "cm/core/print.hpp"


#if !aatc_CONFIG_USE_BOOST
	aatc_std_Spinlock::aatc_std_Spinlock() : state(1) {}
	void aatc_std_Spinlock::lock(){while (state.exchange(0, std::memory_order_acquire) == 0);}
	void aatc_std_Spinlock::unlock(){state.store(1, std::memory_order_release);}
#endif





BEGIN_AS_NAMESPACE

aatc_primunion aatc_primunion_defaultvalue = aatc_primunion();

aatc_hash_type aatc_hashfunc_djb2(const aatc_type_string& a){
	aatc_hash_type hash = 5381;
	for(int i = 0; i < a.size(); i++){
		hash = ((hash << 5) + hash) + a[i];
	}
	return hash;
}

aatc_hash_type aatc_functor_hash<aatc_type_float32>::operator()(const aatc_type_float32& a) const{
	aatc_primunion pu;
	pu.f32 = a;
	return pu.ui64;
}
aatc_hash_type aatc_functor_hash<aatc_type_float64>::operator()(const aatc_type_float64& a) const{
	aatc_primunion pu;
	pu.f64 = a;
	return pu.ui64;
}
aatc_hash_type aatc_functor_hash<aatc_type_string>::operator()(const aatc_type_string& a) const{
	return aatc_hashfunc_djb2(a);
}
#if aatc_CONFIG_USE_ASADDON_REF
	aatc_hash_type aatc_functor_hash<aatc_ait_ref>::operator()(const aatc_ait_ref& a) const{
		return (aatc_hash_type)a.m_ref;
	}
#endif



aatc_engine_level_storage::aatc_engine_level_storage(asIScriptEngine* _engine) :
	engine(_engine),
	objtype_tempcont_list(NULL)
{}
aatc_engine_level_storage::~aatc_engine_level_storage(){}

aatc_template_specific_storage* aatc_containertype_specific_storage::GetTemplateSpecificStorage(aatc_type_uint32 id){
	aatc_template_specific_storage* result;

	//template_specific_storages_lock.lock();
	els->containertype_specific_storages_lock.lock();
		tmap_tss::iterator it = template_specific_storages.find(id);
		if (it == template_specific_storages.end()){
			result = new aatc_template_specific_storage(this,id);
			template_specific_storages.insert(tpair_tss(id, result));
		}else{
			result = it->second;
		}
	els->containertype_specific_storages_lock.unlock();
	//template_specific_storages_lock.unlock();

	return result;
}
aatc_containertype_specific_storage* aatc_engine_level_storage::GetContainerTypeSpecificStorage(aatc_type_uint32 id){
	aatc_containertype_specific_storage* result;

	containertype_specific_storages_lock.lock();
		tmap_ctss::iterator it = containertype_specific_storages.find(id);
		if (it == containertype_specific_storages.end()){
			result = new aatc_containertype_specific_storage(this, id);
			containertype_specific_storages.insert(tpair_ctss(id, result));
		}
		else{
			result = it->second;
		}
	containertype_specific_storages_lock.unlock();

	return result;
}

aatc_containertype_specific_storage::aatc_containertype_specific_storage(aatc_engine_level_storage* _els, aatc_type_uint32 container_id) :
	els(_els),
	container_id(container_id)
{}
aatc_containertype_specific_storage::~aatc_containertype_specific_storage(){
	for (auto it = template_specific_storages.begin(); it != template_specific_storages.end(); it++){
		const auto& pp = *it;
		delete pp.second;
	}
}

void aatc_engine_level_storage::Clean(){
	for (auto it = containertype_specific_storages.begin(); it != containertype_specific_storages.end(); it++){
		const auto& pp = *it;
		delete pp.second;
	}
	containertype_specific_storages.clear();

	for (auto it = context_cache.begin(); it != context_cache.end(); it++){
		auto* a = *it;
		a->Release();

	}
	context_cache.clear();
}

aatc_template_specific_storage::aatc_template_specific_storage(aatc_containertype_specific_storage* ctss,aatc_type_uint32 subtypeid) :
	ctss(ctss),
	func_equals(NULL),
	func_cmp(NULL),
	func_hash(NULL)
{
	asIScriptEngine* engine = asGetActiveContext()->GetEngine();
	asIObjectType* objtype = engine->GetObjectTypeById(subtypeid);

	//get opEquals or opCmp function for this type to be stored

	bool mustBeConst = (subtypeid & asTYPEID_HANDLETOCONST) ? true : false;

	if (objtype){
		for (asUINT i = 0; i < objtype->GetMethodCount(); i++){
			asIScriptFunction *func = objtype->GetMethodByIndex(i);

			asDWORD flags = 0;
			int returnTypeId = func->GetReturnTypeId(&flags);

			if(func->GetParamCount() == 0){
				if(strcmp(func->GetName(), aatc_name_script_requiredmethod_hash) == 0){
					if((returnTypeId == asTYPEID_UINT64) || (returnTypeId == asTYPEID_UINT32) || (returnTypeId == asTYPEID_UINT16) || (returnTypeId == asTYPEID_UINT8)){
						func_hash = func;
					}
				}
			}
			if (func->GetParamCount() == 1 && (!mustBeConst || func->IsReadOnly())){

				// The method must not return a reference
				if (flags != asTM_NONE)
					continue;

				// opCmp returns an int and opEquals returns a bool
				bool isCmp = false, isEq = false;
				if (returnTypeId == asTYPEID_INT32 && strcmp(func->GetName(), "opCmp") == 0){
					isCmp = true;
				}
				if (returnTypeId == asTYPEID_BOOL && strcmp(func->GetName(), "opEquals") == 0){
					isEq = true;
				}

				if(!isCmp && !isEq){
					continue;
				}


				// The parameter must either be a reference to the subtype or a handle to the subtype
				int paramTypeId;
				func->GetParam(0, &paramTypeId, &flags);

				if ((paramTypeId & ~(asTYPEID_OBJHANDLE | asTYPEID_HANDLETOCONST)) != (subtypeid &  ~(asTYPEID_OBJHANDLE | asTYPEID_HANDLETOCONST))){
					continue;
				}

				if ((flags & asTM_INREF)){
					if ((paramTypeId & asTYPEID_OBJHANDLE) || (mustBeConst && !(flags & asTM_CONST))){
						continue;
					}
				}else if (paramTypeId & asTYPEID_OBJHANDLE){
					if (mustBeConst && !(paramTypeId & asTYPEID_HANDLETOCONST)){
						continue;
					}
				}else{
					continue;
				}

				if (isCmp){
					func_cmp = func;
				}else if (isEq){
					func_equals = func;
				}
			}
		}
	}

	missing_functions = aatc_errorcheck_container_type_missing_functions_base(ctss->container_id,this);
}

asIScriptContext* aatc_engine_level_storage::contextcache_Get(){
	asIScriptContext* result;

	context_cache_lock.lock();
		if (context_cache.empty()){
			result = engine->CreateContext();
		}else{
			result = context_cache.back();
			context_cache.pop_back();
		}
	context_cache_lock.unlock();

	return result;
}
void aatc_engine_level_storage::contextcache_Return(asIScriptContext* a){
	context_cache_lock.lock();
		context_cache.push_back(a);
	context_cache_lock.unlock();
}



//refcounted and gc basetype
//refcounted and gc basetype
aatc_refcounted::aatc_refcounted() :
	refcount(1)
{}
aatc_refcounted::~aatc_refcounted(){}
void aatc_refcounted::refcount_Add(){ refcount++; }
void aatc_refcounted::refcount_Release(){
	refcount--;
	if (refcount == 0){
		delete this;
	}
}

aatc_refcounted_GC::aatc_refcounted_GC(asIScriptEngine *engine){
	refCount = 1;
	gcFlag = false;
	this->engine = engine;
}
aatc_refcounted_GC::~aatc_refcounted_GC(){}

void aatc_refcounted_GC::refcount_Add(){
	gcFlag = false;
	asAtomicInc(refCount);
}
void aatc_refcounted_GC::refcount_Release(){
	gcFlag = false;
	if (asAtomicDec(refCount) == 0){
		delete this;
	}
}
int aatc_refcounted_GC::GetRefCount(){return refCount;}
void aatc_refcounted_GC::SetGCFlag(){gcFlag = true;}
bool aatc_refcounted_GC::GetGCFlag(){return gcFlag;}
void aatc_refcounted_GC::EnumReferences(asIScriptEngine *engine){}
void aatc_refcounted_GC::ReleaseAllReferences(asIScriptEngine* engine){}
//refcounted and gc basetype
//refcounted and gc basetype



void aatc_engine_cleanup(asIScriptEngine* engine){
	aatc_engine_level_storage* ss = (aatc_engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);

	ss->Clean();
	delete ss;
}

bool aatc_templatecallback_container(asIObjectType *ot, bool &dontGarbageCollect){
	int typeId = ot->GetSubTypeId();

	if ((typeId & asTYPEID_MASK_OBJECT) && !(typeId & asTYPEID_OBJHANDLE)){
		asIObjectType *subtype = ot->GetEngine()->GetObjectTypeById(typeId);
		asDWORD flags = subtype->GetFlags();
		if ((flags & asOBJ_VALUE) && !(flags & asOBJ_POD)){
			// Verify that there is a default constructor
			bool found = false;
			for (asUINT n = 0; n < subtype->GetBehaviourCount(); n++){
				asEBehaviours beh;
				asIScriptFunction *func = subtype->GetBehaviourByIndex(n, &beh);
				if (beh != asBEHAVE_CONSTRUCT){ continue; }

				if (func->GetParamCount() == 0){
					// Found the default constructor
					found = true;
					break;
				}
			}

			if (!found){
				// There is no default constructor
				return false;
			}
		}
		else if ((flags & asOBJ_REF)){
			bool found = false;

			// If value assignment for ref type has been disabled then the array
			// can be created if the type has a default factory function
			if (!ot->GetEngine()->GetEngineProperty(asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE)){
				// Verify that there is a default factory
				for (asUINT n = 0; n < subtype->GetFactoryCount(); n++){
					asIScriptFunction *func = subtype->GetFactoryByIndex(n);
					if (func->GetParamCount() == 0){
						// Found the default factory
						found = true;
						break;
					}
				}
			}

			if (!found){
				// No default factory
				return false;
			}
		}

		// If the object type is not garbage collected then the array also doesn't need to be
		if (!(flags & asOBJ_GC)){
			dontGarbageCollect = true;
		}
	}
	else if (!(typeId & asTYPEID_OBJHANDLE)){
		// Arrays with primitives cannot form circular references,
		// thus there is no need to garbage collect them
		dontGarbageCollect = true;
	}

	return 1;
}

//do nothing
void aect_iterator_template_generic_constructor_dummydefault(asIObjectType* objtype, void *memory){}

aatc_containerfunctor_comp::aatc_containerfunctor_comp(asIScriptEngine* _engine, aatc_containerfunctor_Settings* settings) :
	engine(_engine),
	host_settings(settings),
	need_init(1)
{
	els = (aatc_engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);
}
bool aatc_containerfunctor_comp::operator()(const void* lhs, const void* rhs)const{
	if (need_init){
		(const_cast<aatc_containerfunctor_comp*>(this))->need_init = 0;
		(const_cast<aatc_containerfunctor_comp*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
		(const_cast<aatc_containerfunctor_comp*>(this))->func_cmp = host_settings->func_cmp;
	}
	if(handlemode_directcomp){
		return lhs < rhs;
	}

	bool result;

	asIScriptContext* cc = els->contextcache_Get();
		cc->Prepare(func_cmp);
		cc->SetObject(const_cast<void*>(lhs));
		cc->SetArgObject(0, const_cast<void*>(rhs));
		cc->Execute();
		//result = (bool)cc->GetReturnByte();
	result = (cc->GetReturnDWord() == -1);

	els->contextcache_Return(cc);

	return result;
}

aatc_containerfunctor_equals::aatc_containerfunctor_equals(asIScriptEngine* _engine, aatc_containerfunctor_Settings* settings) :
	engine(_engine),
	host_settings(settings),
	need_init(1)
{
	els = (aatc_engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);
}
bool aatc_containerfunctor_equals::operator()(const void* lhs, const void* rhs)const{
	if(need_init){
		(const_cast<aatc_containerfunctor_equals*>(this))->need_init = 0;
		(const_cast<aatc_containerfunctor_equals*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
		(const_cast<aatc_containerfunctor_equals*>(this))->func_equals = host_settings->func_equals;
	}
	if(handlemode_directcomp){
		return lhs == rhs;
	}

	bool result;

	asIScriptContext* cc = els->contextcache_Get();
	cc->Prepare(func_equals);
		cc->SetObject(const_cast<void*>(lhs));
		cc->SetArgObject(0, const_cast<void*>(rhs));
	cc->Execute();
	result = (cc->GetReturnDWord() != 0);

	els->contextcache_Return(cc);

	return result;
}

aatc_containerfunctor_hash::aatc_containerfunctor_hash(asIScriptEngine* _engine, aatc_containerfunctor_Settings* settings) :
	engine(_engine),
	host_settings(settings),
	need_init(1)
{
	els = (aatc_engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);
}
aatc_hash_type aatc_containerfunctor_hash::operator()(const void* ptr) const{
	if(need_init){
		//need_init = 0;
		//func_hash = host_settings->func_hash;
		//handlemode_directcomp = host_settings->handlemode_directcomp;
		(const_cast<aatc_containerfunctor_hash*>(this))->need_init = 0;
		(const_cast<aatc_containerfunctor_hash*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
		(const_cast<aatc_containerfunctor_hash*>(this))->func_hash = host_settings->func_hash;
	}
	if(handlemode_directcomp){
		return (aatc_hash_type)ptr;
		//return reinterpret_cast<std::size_t>(ptr);
	}

	aatc_hash_type result;

	asIScriptContext* cc = els->contextcache_Get();
		cc->Prepare(func_hash);
			cc->SetObject(const_cast<void*>(ptr));
		if(cc->Execute() == asEXECUTION_EXCEPTION){
			result = 0;
		}else{
			result = cc->GetReturnQWord();
		}
	els->contextcache_Return(cc);

	return result;
}



//MAP FUNCTORS
//MAP FUNCTORS
//MAP FUNCTORS
aatc_containerfunctor_map_comp::aatc_containerfunctor_map_comp(asIScriptEngine* _engine, aatc_containerfunctor_map_Settings* settings) :
	engine(_engine),
	host_settings(settings),
	need_init(1)
{
	els = (aatc_engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);
}
bool aatc_containerfunctor_map_comp::operator()(const aatc_primunion& lhs, const aatc_primunion& rhs)const{
	if(need_init){
		(const_cast<aatc_containerfunctor_map_comp*>(this))->need_init = 0;
		(const_cast<aatc_containerfunctor_map_comp*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
		(const_cast<aatc_containerfunctor_map_comp*>(this))->datahandlingid_key = host_settings->datahandlingid_key;
		(const_cast<aatc_containerfunctor_map_comp*>(this))->primitiveid_key = host_settings->primitiveid_key;
		(const_cast<aatc_containerfunctor_map_comp*>(this))->func_cmp = host_settings->func_cmp;
	}
	if(datahandlingid_key == aatc_DATAHANDLINGTYPE::PRIMITIVE){
		switch(primitiveid_key){
			case aatc_PRIMITIVE_TYPE::INT8:{return lhs.i8 < rhs.i8; }
			case aatc_PRIMITIVE_TYPE::INT16:{return lhs.i16 < rhs.i16; }
			case aatc_PRIMITIVE_TYPE::INT32:{return lhs.i32 < rhs.i32; }
			case aatc_PRIMITIVE_TYPE::INT64:{return lhs.i64 < rhs.i64; }
			case aatc_PRIMITIVE_TYPE::UINT8:{return lhs.ui8 < rhs.ui8; }
			case aatc_PRIMITIVE_TYPE::UINT16:{return lhs.ui16 < rhs.ui16; }
			case aatc_PRIMITIVE_TYPE::UINT32:{return lhs.ui32 < rhs.ui32; }
			case aatc_PRIMITIVE_TYPE::UINT64:{return lhs.ui64 < rhs.ui64; }
			case aatc_PRIMITIVE_TYPE::FLOAT32:{return lhs.f32 < rhs.f32; }
			case aatc_PRIMITIVE_TYPE::FLOAT64:{return lhs.f64 < rhs.f64; }
		};
	}
	if(datahandlingid_key == aatc_DATAHANDLINGTYPE::STRING){
		return (*((aatc_type_string*)lhs.ptr)) < (*((aatc_type_string*)rhs.ptr));
	}
	if(handlemode_directcomp){
		return lhs.ptr < rhs.ptr;
	}

	bool result;

	asIScriptContext* cc = els->contextcache_Get();
	cc->Prepare(func_cmp);
	cc->SetObject(const_cast<void*>(lhs.ptr));
	cc->SetArgObject(0, const_cast<void*>(rhs.ptr));
	cc->Execute();
	//result = (bool)cc->GetReturnByte();
	result = (cc->GetReturnDWord() == -1);

	els->contextcache_Return(cc);

	return result;
}


aatc_containerfunctor_map_equals::aatc_containerfunctor_map_equals(asIScriptEngine* _engine, aatc_containerfunctor_map_Settings* settings) :
	engine(_engine),
	host_settings(settings),
	need_init(1)
{
	els = (aatc_engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);
}
bool aatc_containerfunctor_map_equals::operator()(const aatc_primunion& lhs, const aatc_primunion& rhs)const{
	if(need_init){
		(const_cast<aatc_containerfunctor_map_equals*>(this))->need_init = 0;
		(const_cast<aatc_containerfunctor_map_equals*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
		(const_cast<aatc_containerfunctor_map_equals*>(this))->datahandlingid_key = host_settings->datahandlingid_key;
		(const_cast<aatc_containerfunctor_map_equals*>(this))->primitiveid_key = host_settings->primitiveid_key;
		(const_cast<aatc_containerfunctor_map_equals*>(this))->func_equals = host_settings->func_equals;
	}
	if(datahandlingid_key == aatc_DATAHANDLINGTYPE::PRIMITIVE){
		switch(primitiveid_key){
			case aatc_PRIMITIVE_TYPE::INT8:{return lhs.i8 == rhs.i8; }
			case aatc_PRIMITIVE_TYPE::INT16:{return lhs.i16 == rhs.i16; }
			case aatc_PRIMITIVE_TYPE::INT32:{return lhs.i32 == rhs.i32; }
			case aatc_PRIMITIVE_TYPE::INT64:{return lhs.i64 == rhs.i64; }
			case aatc_PRIMITIVE_TYPE::UINT8:{return lhs.ui8 == rhs.ui8; }
			case aatc_PRIMITIVE_TYPE::UINT16:{return lhs.ui16 == rhs.ui16; }
			case aatc_PRIMITIVE_TYPE::UINT32:{return lhs.ui32 == rhs.ui32; }
			case aatc_PRIMITIVE_TYPE::UINT64:{return lhs.ui64 == rhs.ui64; }
			case aatc_PRIMITIVE_TYPE::FLOAT32:{return lhs.f32 == rhs.f32; }
			case aatc_PRIMITIVE_TYPE::FLOAT64:{return lhs.f64 == rhs.f64; }
		};
	}
	if(datahandlingid_key == aatc_DATAHANDLINGTYPE::STRING){
		return (*((aatc_type_string*)lhs.ptr)) == (*((aatc_type_string*)rhs.ptr));
	}
	if(handlemode_directcomp){
		return lhs.ptr == rhs.ptr;
	}

	bool result;

	asIScriptContext* cc = els->contextcache_Get();
	cc->Prepare(func_equals);
	cc->SetObject(const_cast<void*>(lhs.ptr));
	cc->SetArgObject(0, const_cast<void*>(rhs.ptr));
	cc->Execute();
	result = (cc->GetReturnDWord() != 0);

	els->contextcache_Return(cc);

	return result;
}

aatc_containerfunctor_map_hash::aatc_containerfunctor_map_hash(asIScriptEngine* _engine, aatc_containerfunctor_map_Settings* settings) :
engine(_engine),
host_settings(settings),
need_init(1)
{
	els = (aatc_engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);
}
aatc_hash_type aatc_containerfunctor_map_hash::operator()(const aatc_primunion& pu) const{
	if(need_init){
		//need_init = 0;
		//func_hash = host_settings->func_hash;
		//handlemode_directcomp = host_settings->handlemode_directcomp;
		(const_cast<aatc_containerfunctor_map_hash*>(this))->need_init = 0;
		(const_cast<aatc_containerfunctor_map_hash*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
		(const_cast<aatc_containerfunctor_map_hash*>(this))->datahandlingid_key = host_settings->datahandlingid_key;
		(const_cast<aatc_containerfunctor_map_hash*>(this))->primitiveid_key = host_settings->primitiveid_key;
		(const_cast<aatc_containerfunctor_map_hash*>(this))->func_hash = host_settings->func_hash;
	}
	if(datahandlingid_key == aatc_DATAHANDLINGTYPE::PRIMITIVE){
		switch(primitiveid_key){
			case aatc_PRIMITIVE_TYPE::INT8:{return (aatc_hash_type)pu.i8; }
			case aatc_PRIMITIVE_TYPE::INT16:{return (aatc_hash_type)pu.i16; }
			case aatc_PRIMITIVE_TYPE::INT32:{return (aatc_hash_type)pu.i32; }
			case aatc_PRIMITIVE_TYPE::INT64:{return (aatc_hash_type)pu.i64; }
			case aatc_PRIMITIVE_TYPE::UINT8:{return (aatc_hash_type)pu.ui8; }
			case aatc_PRIMITIVE_TYPE::UINT16:{return (aatc_hash_type)pu.ui16; }
			case aatc_PRIMITIVE_TYPE::UINT32:{return (aatc_hash_type)pu.ui32; }
			case aatc_PRIMITIVE_TYPE::UINT64:{return (aatc_hash_type)pu.ui64; }
			case aatc_PRIMITIVE_TYPE::FLOAT32:{return (aatc_hash_type)pu.f32; }
			case aatc_PRIMITIVE_TYPE::FLOAT64:{return (aatc_hash_type)pu.f64; }
		};
	}
	if(datahandlingid_key == aatc_DATAHANDLINGTYPE::STRING){
		return aatc_functor_hash<aatc_type_string>()(*((aatc_type_string*)pu.ptr));
	}
	if(handlemode_directcomp){
		return (aatc_hash_type)pu.ptr;
		//return reinterpret_cast<std::size_t>(ptr);
	}

	aatc_hash_type result;

	asIScriptContext* cc = els->contextcache_Get();
	cc->Prepare(func_hash);
	cc->SetObject(const_cast<void*>(pu.ptr));
	if(cc->Execute() == asEXECUTION_EXCEPTION){
		result = 0;
	} else{
		result = cc->GetReturnQWord();
	}
	els->contextcache_Return(cc);

	return result;
}
//MAP FUNCTORS
//MAP FUNCTORS
//MAP FUNCTORS





aatc_script_Funcpointer::aatc_script_Funcpointer() :
	//dedicated_context(NULL),
	ready(0),
	func(NULL)
{
}
aatc_script_Funcpointer::~aatc_script_Funcpointer(){
	//ReleaseRef();
}

aatc_script_Funcpointer* aatc_script_Funcpointer::Factory(){
	return new aatc_script_Funcpointer();
}

bool aatc_script_Funcpointer::Set(aatc_type_string _funcname){
	asIScriptContext* ctx = asGetActiveContext();
	engine = ctx->GetEngine();

	funcname = _funcname;
	ready = 0;
	func = NULL;
	is_thiscall = 0;

	const char* funcname_c = funcname.c_str();

	//application registered global functions
	func = engine->GetGlobalFunctionByDecl(funcname_c);
	//not found? script module global functions
	if (!func){
		for (uint_fast32_t i = 0; i < engine->GetModuleCount(); i++){
			func = engine->GetModuleByIndex(i)->GetFunctionByName(funcname_c);
			if (func){ break; }
		}
	}

	if (func){
		ready = 1;
	}
	return ready;
}
bool aatc_script_Funcpointer::Set(aatc_type_string _funcname, void* ref, int tid){
	funcname = _funcname;
	ready = 0;
	func = NULL;
	is_thiscall = 1;

	const char* funcname_c = funcname.c_str();

	if (ref){
		if ((tid & asTYPEID_MASK_OBJECT) && (tid & asTYPEID_OBJHANDLE)){//must be not primitive and a handle
			so = static_cast<asIScriptObject*>(*((void**)ref));

			func = so->GetObjectType()->GetMethodByName(funcname_c);
			if (func){
				ready = 1;
			}
		}else{//error
			asIScriptContext* ctx = asGetActiveContext();
			engine = ctx->GetEngine();

			char textbuf[1024];
			sprintf_s(textbuf,1024, aatc_errormessage_funcpointer_nothandle,engine->GetObjectTypeById(tid)->GetName());
			ctx->SetException(textbuf);
		}
	}
	return ready;
}
void aatc_script_Funcpointer::Clear(){
	ready = 0;
}

void aatc_script_Funcpointer::Prepare(asIScriptContext* context){
	if (ready){
		context->Prepare(func);
		if (is_thiscall){context->SetObject(so);}
	}
}
void aatc_script_Funcpointer::Execute(asIScriptContext* context){
	if (ready){
		context->Execute();
	}
}

void aatc_script_Funcpointer::scriptsidecall_CallVoid(){
	aatc_engine_level_storage* els = (aatc_engine_level_storage*)asGetActiveContext()->GetEngine()->GetUserData(aatc_engine_userdata_id);
	asIScriptContext* c = els->contextcache_Get();
		Prepare(c);
		Execute(c);
	els->contextcache_Return(c);
}

//void aatc_script_Funcpointer::ReleaseRef(){
//	ref.Set(NULL, NULL);
//}

asIScriptContext* aatc_contextcache_Get(){
	aatc_engine_level_storage* els = (aatc_engine_level_storage*)asGetActiveContext()->GetEngine()->GetUserData(aatc_engine_userdata_id);
	return els->contextcache_Get();
}
void aatc_contextcache_Return(asIScriptContext* c){
	aatc_engine_level_storage* els = (aatc_engine_level_storage*)asGetActiveContext()->GetEngine()->GetUserData(aatc_engine_userdata_id);
	els->contextcache_Return(c);
}


void aatc_errorprint_container_missingfunctions_operation_missing(const char* name_container, const char* name_content, const char* name_operation){
#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_EXCEPTIONS
	char msg[1000];
	//sprintf_s(msg, 1000, aatc_errormessage_container_missing_method_formatting, name_content, name_container, name_operation);
	sprintf_s(msg, 1000, aatc_errormessage_container_missingfunctions_formatting, aatc_errormessage_container_missingfunctions_formatting_param1, aatc_errormessage_container_missingfunctions_formatting_param2, aatc_errormessage_container_missingfunctions_formatting_param3);
	asGetActiveContext()->SetException(msg);
#endif
}
void aatc_errorprint_container_access_empty(const char* name_container, const char* name_content, const char* name_operation){
#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_EXCEPTIONS
	char msg[1000];
	//sprintf_s(msg, 1000, aatc_errormessage_container_missing_method_formatting, name_content, name_container, name_operation);
	sprintf_s(msg, 1000, aatc_errormessage_container_access_empty_formatting, aatc_errormessage_container_access_empty_formatting_param1, aatc_errormessage_container_access_empty_formatting_param2, aatc_errormessage_container_access_empty_formatting_param3);
	asGetActiveContext()->SetException(msg);
#endif
}
void aatc_errorprint_container_access_bounds(aatc_type_sizetype index, aatc_type_sizetype size, const char* name_container, const char* name_content, const char* name_operation){
#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_EXCEPTIONS
	char msg[1000];
	//sprintf_s(msg, 1000, aatc_errormessage_container_missing_method_formatting, name_content, name_container, name_operation);
	sprintf_s(msg, 1000,
		aatc_errormessage_container_access_bounds_formatting,
		aatc_errormessage_container_access_bounds_formatting_param1,
		aatc_errormessage_container_access_bounds_formatting_param2,
		aatc_errormessage_container_access_bounds_formatting_param3,
		aatc_errormessage_container_access_bounds_formatting_param4,
		aatc_errormessage_container_access_bounds_formatting_param5
	);
	asGetActiveContext()->SetException(msg);
#endif
}



aatc_DATAHANDLINGTYPE aatc_Determine_Datahandlingtype(aatc_type_uint32 astypeid){
	if(astypeid == asGetActiveContext()->GetEngine()->GetStringFactoryReturnTypeId()){
		return aatc_DATAHANDLINGTYPE::STRING;
	}
	if(astypeid & asTYPEID_MASK_OBJECT){
		if(astypeid & asTYPEID_OBJHANDLE){
			return aatc_DATAHANDLINGTYPE::HANDLE;
		} else{
			return aatc_DATAHANDLINGTYPE::OBJECT;
		}
	} else{
		return aatc_DATAHANDLINGTYPE::PRIMITIVE;
	}
}
aatc_PRIMITIVE_TYPE aatc_Determine_Primitivetype(aatc_type_uint32 astypeid){
	switch(astypeid){
		case asTYPEID_BOOL:{return aatc_PRIMITIVE_TYPE::INT8; }
		case asTYPEID_INT8:{return aatc_PRIMITIVE_TYPE::INT8; }
		case asTYPEID_INT16:{return aatc_PRIMITIVE_TYPE::INT16; }
		case asTYPEID_INT32:{return aatc_PRIMITIVE_TYPE::INT32; }
		case asTYPEID_INT64:{return aatc_PRIMITIVE_TYPE::INT64; }
		case asTYPEID_UINT8:{return aatc_PRIMITIVE_TYPE::UINT8; }
		case asTYPEID_UINT16:{return aatc_PRIMITIVE_TYPE::UINT16; }
		case asTYPEID_UINT32:{return aatc_PRIMITIVE_TYPE::UINT32; }
		case asTYPEID_UINT64:{return aatc_PRIMITIVE_TYPE::UINT64; }
		case asTYPEID_FLOAT:{return aatc_PRIMITIVE_TYPE::FLOAT32; }
		case asTYPEID_DOUBLE:{return aatc_PRIMITIVE_TYPE::FLOAT64; }
		default: {return aatc_PRIMITIVE_TYPE::INT8; }
	};
}

aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions_base(int CONTAINER_ID, aatc_template_specific_storage* tss){
	switch(CONTAINER_ID){
		case aatc_CONTAINERTYPE::VECTOR: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::VECTOR>(tss);
		case aatc_CONTAINERTYPE::LIST: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::LIST>(tss);
		case aatc_CONTAINERTYPE::SET: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::SET>(tss);
		case aatc_CONTAINERTYPE::UNORDERED_SET: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::UNORDERED_SET>(tss);
		case aatc_CONTAINERTYPE::MAP: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::MAP>(tss);
		case aatc_CONTAINERTYPE::UNORDERED_MAP: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::UNORDERED_MAP>(tss);
	};
	return 0;
}





END_AS_NAMESPACE