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


#ifndef _includedh_aatc_shared_map_template
#define _includedh_aatc_shared_map_template



#include "aatc_common.hpp"
//#include "cm/core/print.hpp"



BEGIN_AS_NAMESPACE




//doxygen skip
#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*
	We need a wrapper around the base container so that we can
	feed the constructor a fancy functor to handle internal functions that require script functions to be called
*/

template<class T_container> class aatc_bcw_shared_map_basic : public T_container{
public:
	aatc_bcw_shared_map_basic(asIScriptEngine* engine, aatc_containerfunctor_map_Settings* settings){}
};

template<class T_container, class T_param1> class aatc_bcwshared_map_1_param : public T_container{
public:
	aatc_bcwshared_map_1_param(asIScriptEngine* _engine, aatc_containerfunctor_map_Settings* settings) :
		T_container(T_param1(_engine, settings))
	{}
};

template<class T_container, class T_param1, class T_param2, std::size_t default_bucket_count> class aatc_shared_map_bcw_hashmap : public T_container{
public:
	aatc_shared_map_bcw_hashmap(asIScriptEngine* _engine, aatc_containerfunctor_map_Settings* settings) :
		T_container(default_bucket_count,T_param1(_engine, settings), T_param2(_engine, settings))
	{}
};

#endif
//doxygen skip



/*!\brief Internal template monster

*/
template<	
	class T_container,
	int T_CONTAINERTYPEID,
	class bcw = aatc_bcw_shared_map_basic<T_container>
>
class aatc_container_shared_map_template : public bcw,
	public aatc_refcounted_GC,
	public aatc_containerfunctor_map_Settings
{
public:
	asIScriptEngine* engine;
	aatc_engine_level_storage* els;

	aatc_DATAHANDLINGTYPE datahandlingid_value;
	aatc_PRIMITIVE_TYPE primitiveid_value;

	asIObjectType* objtype_container;
	asIObjectType* objtype_key;
	asIObjectType* objtype_value;

	bool objectmode_key;
	bool objectmode_value;
	bool needref_key;
	bool needref_value;

	aatc_container_operations_bitmask_type missing_functions;

	bool directcomp_forced;
	bool need_errorcheck_missing_functions;

	aatc_container_shared_map_template(asIScriptEngine* _engine, asIObjectType* _objtype) :
		bcw(_engine, this),
		aatc_refcounted_GC(_engine),
		engine(_engine),
		objtype_container(_objtype),
		needref_key(1),
		needref_value(1),
		directcomp_forced(0),
		need_errorcheck_missing_functions(1)
	{
		objtype_container->AddRef();

		int astypeid_key = objtype_container->GetSubTypeId(0);
		int astypeid_value = objtype_container->GetSubTypeId(1);

		datahandlingid_key = aatc_Determine_Datahandlingtype(astypeid_key);
		datahandlingid_value = aatc_Determine_Datahandlingtype(astypeid_value);

		handlemode_directcomp = 0;

		switch(datahandlingid_key){
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
				primitiveid_key = aatc_Determine_Primitivetype(astypeid_key);
				need_errorcheck_missing_functions = 0;
				objectmode_key = 0;
			break; }
			case aatc_DATAHANDLINGTYPE::STRING:{
				need_errorcheck_missing_functions = 0;
				objectmode_key = 0;
				objtype_key = engine->GetObjectTypeById(engine->GetStringFactoryReturnTypeId());
			break; }
			default:{
				objectmode_key = 1;

				if(datahandlingid_key == aatc_DATAHANDLINGTYPE::HANDLE){
					handlemode_directcomp = aatc_CONFIG_DEFAULT_HANDLEMODE_DIRECTCOMP;
				}

				if(handlemode_directcomp){
					need_errorcheck_missing_functions = 0;
				}

				objtype_key = objtype_container->GetSubType(0);
				objtype_key->AddRef();
				if(astypeid_key & asOBJ_NOCOUNT){ needref_key = 0; }
			break; }
		};
		switch(datahandlingid_value){
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
													  objectmode_value = 0;
													  primitiveid_value = aatc_Determine_Primitivetype(astypeid_value);
													  break; }
			case aatc_DATAHANDLINGTYPE::STRING:{
												   objectmode_value = 0;
												   objtype_value = engine->GetObjectTypeById(engine->GetStringFactoryReturnTypeId());
												   break; }
			default:{
						objectmode_value = 1;
						objtype_value = objtype_container->GetSubType(1);
						objtype_value->AddRef();
						if(astypeid_value & asOBJ_NOCOUNT){ needref_value = 0; }
						break; }
		};

		els = (aatc_engine_level_storage*)asGetActiveContext()->GetEngine()->GetUserData(aatc_engine_userdata_id);
		aatc_containertype_specific_storage* ctss = els->GetContainerTypeSpecificStorage(T_CONTAINERTYPEID);

		aatc_template_specific_storage* tss = ctss->GetTemplateSpecificStorage(astypeid_key);
		func_equals = tss->func_equals;
		func_cmp = tss->func_cmp;
		func_hash = tss->func_hash;

		missing_functions = tss->missing_functions;

		if(need_errorcheck_missing_functions){
#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE
			if(missing_functions){
				bool bad = 1;

#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE_USE_DIRECTCOMP
				if(datahandlingid_key == aatc_DATAHANDLINGTYPE::HANDLE){//force handlemode directcomp if anything is missing
					handlemode_directcomp = 1;
					directcomp_forced = 1;
					bad = 0;
				}
#endif

				if(bad){
					char msg[1000];
					sprintf_s(msg, 1000, "Type '%s' has missing methods required for container '%s'.", objtype_content->GetName(), objtype_container->GetName());
					asGetActiveContext()->SetException(msg);
				}
			}
#endif
		}

		engine->NotifyGarbageCollectorOfNewObject(this, objtype_container);
	}
	~aatc_container_shared_map_template(){
		Clear();

		objtype_container->Release();

		if(objectmode_value){
			objtype_key->Release();
		}
		if(objectmode_value){
			objtype_value->Release();
		}
	}

	void operator=(const aatc_container_shared_map_template& other){
		Clear();//will delete script objects or decrement handles

		handlemode_directcomp = other.handlemode_directcomp;
		directcomp_forced = other.directcomp_forced;

		if(!objectmode_key){//use simple copy without inserts
			T_container::operator=(other);

			if(datahandlingid_key == aatc_DATAHANDLINGTYPE::STRING){
				T_container::iterator it = T_container::begin();
				T_container::iterator it_end = T_container::end();

				for(; it != it_end; it++){
					void*& iii = const_cast<void*&>((*it).first.ptr);
					iii = engine->CreateScriptObjectCopy(iii, objtype_key);
				}
			}

			switch(datahandlingid_value){
			case aatc_DATAHANDLINGTYPE::STRING:{
												   T_container::iterator it = T_container::begin();
												   T_container::iterator it_other = const_cast<aatc_container_shared_map_template&>(other).begin();
												   T_container::iterator it_end = T_container::end();

												   for(; it != it_end;){
													   void*& iii = const_cast<void*&>((*it).second.ptr);
													   void*& iii_other = const_cast<void*&>((*it_other).second.ptr);

													   iii = engine->CreateScriptObjectCopy(iii_other, objtype_value);

													   it++;
													   it_other++;
												   }
												   break; }
			case aatc_DATAHANDLINGTYPE::HANDLE:{
												   for(auto it = T_container::begin(); it != T_container::end(); it++){
													   engine->AddRefScriptObject((*it).second.ptr, objtype_value);
												   }
												   break; }
			case aatc_DATAHANDLINGTYPE::OBJECT:{
												   T_container::iterator it = T_container::begin();
												   T_container::iterator it_other = const_cast<aatc_container_shared_map_template&>(other).begin();
												   T_container::iterator it_end = T_container::end();

												   for(; it != it_end;){
													   void*& iii = const_cast<void*&>((*it).second.ptr);
													   void*& iii_other = const_cast<void*&>((*it_other).second.ptr);

													   iii = engine->CreateScriptObjectCopy(iii_other, objtype_value);

													   it++;
													   it_other++;
												   }
												   break; }
			};
		} else{//copy by inserting everything, takes time
			for(auto it = other.begin(); it != other.end(); it++){
				aatc_primunion& otherpu_key = const_cast<aatc_primunion&>((*it).first);
				aatc_primunion& otherpu_value = const_cast<aatc_primunion&>((*it).second);

				aatc_primunion pu_key;
				aatc_primunion pu_value;

				switch(datahandlingid_key){
				case aatc_DATAHANDLINGTYPE::HANDLE:{
													   pu_key.ptr = otherpu_key.ptr;
													   engine->AddRefScriptObject(pu_key.ptr, objtype_key);
													   break; }
				case aatc_DATAHANDLINGTYPE::OBJECT:{
													   pu_key.ptr = engine->CreateScriptObjectCopy(otherpu_key.ptr, objtype_key);
													   break; }
				case aatc_DATAHANDLINGTYPE::STRING:{
													   pu_key.ptr = engine->CreateScriptObjectCopy(otherpu_key.ptr, objtype_key);
													   break; }
				case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
														  switch(primitiveid_key){
														  case aatc_PRIMITIVE_TYPE::INT8:{pu_key.i8 = otherpu_key.i8; break; }
														  case aatc_PRIMITIVE_TYPE::INT16:{pu_key.i16 = otherpu_key.i16; break; }
														  case aatc_PRIMITIVE_TYPE::INT32:{pu_key.i32 = otherpu_key.i32; break; }
														  case aatc_PRIMITIVE_TYPE::INT64:{pu_key.i64 = otherpu_key.i64; break; }
														  case aatc_PRIMITIVE_TYPE::UINT8:{pu_key.ui8 = otherpu_key.ui8; break; }
														  case aatc_PRIMITIVE_TYPE::UINT16:{pu_key.ui16 = otherpu_key.ui16; break; }
														  case aatc_PRIMITIVE_TYPE::UINT32:{pu_key.ui32 = otherpu_key.ui32; break; }
														  case aatc_PRIMITIVE_TYPE::UINT64:{pu_key.ui64 = otherpu_key.ui64; break; }
														  case aatc_PRIMITIVE_TYPE::FLOAT32:{pu_key.f32 = otherpu_key.f32; break; }
														  case aatc_PRIMITIVE_TYPE::FLOAT64:{pu_key.f64 = otherpu_key.f64; break; }
														  };
														  break; }
				};

				switch(datahandlingid_value){
				case aatc_DATAHANDLINGTYPE::HANDLE:{
													   pu_value.ptr = otherpu_value.ptr;
													   engine->AddRefScriptObject(pu_value.ptr, objtype_value);
													   break; }
				case aatc_DATAHANDLINGTYPE::OBJECT:{
													   pu_value.ptr = engine->CreateScriptObjectCopy(otherpu_value.ptr, objtype_value);
													   break; }
				case aatc_DATAHANDLINGTYPE::STRING:{
													   pu_value.ptr = engine->CreateScriptObjectCopy(otherpu_value.ptr, objtype_value);
													   break; }
				case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
														  switch(primitiveid_value){
														  case aatc_PRIMITIVE_TYPE::INT8:{pu_value.i8 = otherpu_value.i8; break; }
														  case aatc_PRIMITIVE_TYPE::INT16:{pu_value.i16 = otherpu_value.i16; break; }
														  case aatc_PRIMITIVE_TYPE::INT32:{pu_value.i32 = otherpu_value.i32; break; }
														  case aatc_PRIMITIVE_TYPE::INT64:{pu_value.i64 = otherpu_value.i64; break; }
														  case aatc_PRIMITIVE_TYPE::UINT8:{pu_value.ui8 = otherpu_value.ui8; break; }
														  case aatc_PRIMITIVE_TYPE::UINT16:{pu_value.ui16 = otherpu_value.ui16; break; }
														  case aatc_PRIMITIVE_TYPE::UINT32:{pu_value.ui32 = otherpu_value.ui32; break; }
														  case aatc_PRIMITIVE_TYPE::UINT64:{pu_value.ui64 = otherpu_value.ui64; break; }
														  case aatc_PRIMITIVE_TYPE::FLOAT32:{pu_value.f32 = otherpu_value.f32; break; }
														  case aatc_PRIMITIVE_TYPE::FLOAT64:{pu_value.f64 = otherpu_value.f64; break; }
														  };
														  break; }
				};

				T_container::insert(std::pair<aatc_primunion, aatc_primunion>(pu_key, pu_value));
			}
		}
	}


	void EnumReferences(asIScriptEngine* engine){
		if(objectmode_key){
			T_container::iterator it = T_container::begin();
			T_container::iterator itend = T_container::end();
			for(; it != itend; it++){
				engine->GCEnumCallback((*it).first.ptr);
			}
		}
		if(objectmode_value){
			T_container::iterator it = T_container::begin();
			T_container::iterator itend = T_container::end();
			for(; it != itend; it++){
				engine->GCEnumCallback((*it).second.ptr);
			}
		}
	}
	void ReleaseAllReferences(asIScriptEngine* engine){
		Clear();
	}

	void SetDirectcomp(bool ss){
		/*
			Dont allow setting if it was forced to 1,
			only thing that forces it is
			aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE
			check failure and it does it to prevent crashing.
			*/
		if(directcomp_forced){ return; }
		if(datahandlingid_key == aatc_DATAHANDLINGTYPE::HANDLE){
			handlemode_directcomp = ss;

			if(handlemode_directcomp){
				need_errorcheck_missing_functions = 0;
			} else{
				need_errorcheck_missing_functions = 1;
			}
		}
	}



	//handle input is void* that points at handle which is a void*
	//handle output is void* that points at handle which is a void*

	void StoreHandle(void** target, void* ptr_to_handle, asIObjectType* objtype){
		//*target = *(void**)ptr_to_handle;
		*target = ptr_to_handle;
		//if (handlemode_needref){
		engine->AddRefScriptObject(*target, objtype);
		//engine->AddRefScriptObject(ptr_to_handle, objtype);
		//}
	}
	void* StoreHandle2(void* ptr_to_handle, asIObjectType* objtype){
		void* result = *(void**)ptr_to_handle;
		engine->AddRefScriptObject(result, objtype);
		return result;
	}
	//void ReleaseHandle(void* handle){
	//	//if (handlemode_needref){
	//		engine->ReleaseScriptObject(handle, objtype_content);
	//	//}
	//}

	void BuildPrimunion(aatc_primunion& pu, void* inputvalue, const aatc_DATAHANDLINGTYPE& dht, const aatc_PRIMITIVE_TYPE& pt){
		switch(dht){
			case aatc_DATAHANDLINGTYPE::HANDLE:{
												   //if(inputvalue){
													   pu.ptr = *(void**)inputvalue;
												   //} else{
													  // pu.ptr = nullptr;
												   //}
													   break; }
			case aatc_DATAHANDLINGTYPE::OBJECT:{
												   pu.ptr = inputvalue;
												   break; }
			case aatc_DATAHANDLINGTYPE::STRING:{
												   pu.ptr = inputvalue;
												   break; }
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
				switch(pt){
					case aatc_PRIMITIVE_TYPE::INT8:{pu.i8 = *((aatc_type_int8*)inputvalue); break; }
					case aatc_PRIMITIVE_TYPE::INT16:{pu.i16 = *((aatc_type_int16*)inputvalue); break; }
					case aatc_PRIMITIVE_TYPE::INT32:{pu.i32 = *((aatc_type_int32*)inputvalue); break; }
					case aatc_PRIMITIVE_TYPE::INT64:{pu.i64 = *((aatc_type_int64*)inputvalue); break; }
					case aatc_PRIMITIVE_TYPE::UINT8:{pu.ui8 = *((aatc_type_uint8*)inputvalue); break; }
					case aatc_PRIMITIVE_TYPE::UINT16:{pu.ui16 = *((aatc_type_uint16*)inputvalue); break; }
					case aatc_PRIMITIVE_TYPE::UINT32:{pu.ui32 = *((aatc_type_uint32*)inputvalue); break; }
					case aatc_PRIMITIVE_TYPE::UINT64:{pu.ui64 = *((aatc_type_uint64*)inputvalue); break; }
					case aatc_PRIMITIVE_TYPE::FLOAT32:{pu.f32 = *((aatc_type_float32*)inputvalue); break; }
					case aatc_PRIMITIVE_TYPE::FLOAT64:{pu.f64 = *((aatc_type_float64*)inputvalue); break; }
				};
			break; }
		};
	}

	void Clear(){
		switch(datahandlingid_key){
		case aatc_DATAHANDLINGTYPE::STRING:{
											   auto it = T_container::begin();
											   auto itend = T_container::end();
											   for(; it != itend; it++){
												   //delete (aatc_type_string*)(const_cast<aatc_primunion&>((*it).first).ptr);
												   engine->ReleaseScriptObject((*it).first.ptr, objtype_key);
											   }
											   break; }
		case aatc_DATAHANDLINGTYPE::HANDLE:{
											   auto it = T_container::begin();
											   auto itend = T_container::end();
											   for(; it != itend; it++){
												   engine->ReleaseScriptObject((*it).first.ptr, objtype_key);
												   //ReleaseHandle((*it).first);
											   }
											   break; }
		case aatc_DATAHANDLINGTYPE::OBJECT:{
											   auto it = T_container::begin();
											   auto itend = T_container::end();
											   for(; it != itend; it++){
												   engine->ReleaseScriptObject((*it).first.ptr, objtype_key);
											   }
											   break; }
		};
		switch(datahandlingid_value){
		case aatc_DATAHANDLINGTYPE::STRING:{
											   auto it = T_container::begin();
											   auto itend = T_container::end();
											   for(; it != itend; it++){
												   //delete (aatc_type_string*)(const_cast<aatc_primunion&>((*it).second).ptr);
												   engine->ReleaseScriptObject((*it).second.ptr, objtype_value);
											   }
											   break; }
		case aatc_DATAHANDLINGTYPE::HANDLE:{
											   auto it = T_container::begin();
											   auto itend = T_container::end();
											   for(; it != itend; it++){
												   //ReleaseHandle((*it).second);
												   engine->ReleaseScriptObject((*it).second.ptr, objtype_value);
											   }
											   break; }
		case aatc_DATAHANDLINGTYPE::OBJECT:{
											   auto it = T_container::begin();
											   auto itend = T_container::end();
											   for(; it != itend; it++){
												   engine->ReleaseScriptObject((*it).second.ptr, objtype_value);
											   }
											   break; }
		};

		T_container::clear();
	}
	bool Empty(){ return T_container::empty(); }
	aatc_type_sizetype Size(){ return (aatc_type_sizetype)(T_container::size()); }



	void Insert(void* newkey, void* newvalue){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
			if(need_errorcheck_missing_functions){
				aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::INSERT, objtype_container->GetName(), objtype_key->GetName(), "insert")
			}
		#endif

		//if(datahandlingid_key == aatc_DATAHANDLINGTYPE::HANDLE){
		//	if(!newkey){
		//		return;
		//	}
		//}

		aatc_primunion findkey;

		BuildPrimunion(findkey, newkey, datahandlingid_key, primitiveid_key);
		//if(datahandlingid_key == aatc_DATAHANDLINGTYPE::STRING){findkey.ptr = newkey;}

		T_container::iterator it = T_container::find(findkey);
		if(it == T_container::end()){

			std::pair<aatc_primunion, aatc_primunion> insertpair;

			switch(datahandlingid_key){
			case aatc_DATAHANDLINGTYPE::STRING:{
												   //insertpair.first.ptr = new aatc_type_string(*((aatc_type_string*)newkey));
												   insertpair.first.ptr = engine->CreateScriptObjectCopy(newkey, objtype_key);
												   break; }
			case aatc_DATAHANDLINGTYPE::HANDLE:{
												   newkey = *(void**)newkey;
												   StoreHandle(&(insertpair.first.ptr), newkey, objtype_key);
												   break; }
			case aatc_DATAHANDLINGTYPE::OBJECT:{
												   insertpair.first.ptr = engine->CreateScriptObjectCopy(newkey, objtype_key);
												   break; }
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
													  switch(primitiveid_key){
													  case aatc_PRIMITIVE_TYPE::INT8:{insertpair.first.i8 = *((aatc_type_int8*)newkey); break; }
													  case aatc_PRIMITIVE_TYPE::INT16:{insertpair.first.i16 = *((aatc_type_int16*)newkey); break; }
													  case aatc_PRIMITIVE_TYPE::INT32:{insertpair.first.i32 = *((aatc_type_int32*)newkey); break; }
													  case aatc_PRIMITIVE_TYPE::INT64:{insertpair.first.i64 = *((aatc_type_int64*)newkey); break; }
													  case aatc_PRIMITIVE_TYPE::UINT8:{insertpair.first.ui8 = *((aatc_type_uint8*)newkey); break; }
													  case aatc_PRIMITIVE_TYPE::UINT16:{insertpair.first.ui16 = *((aatc_type_uint16*)newkey); break; }
													  case aatc_PRIMITIVE_TYPE::UINT32:{insertpair.first.ui32 = *((aatc_type_uint32*)newkey); break; }
													  case aatc_PRIMITIVE_TYPE::UINT64:{insertpair.first.ui64 = *((aatc_type_uint64*)newkey); break; }
													  case aatc_PRIMITIVE_TYPE::FLOAT32:{insertpair.first.f32 = *((aatc_type_float32*)newkey); break; }
													  case aatc_PRIMITIVE_TYPE::FLOAT64:{insertpair.first.f64 = *((aatc_type_float64*)newkey); break; }
													  };
													  break; }
			};
			switch(datahandlingid_value){
			case aatc_DATAHANDLINGTYPE::STRING:{
												   //insertpair.second.ptr = new aatc_type_string(*((aatc_type_string*)newvalue));
												   insertpair.second.ptr = engine->CreateScriptObjectCopy(newvalue, objtype_value);
												   break; }
			case aatc_DATAHANDLINGTYPE::HANDLE:{
												   newvalue = *(void**)newvalue;
												   StoreHandle(&(insertpair.second.ptr), newvalue, objtype_value);
												   break; }
			case aatc_DATAHANDLINGTYPE::OBJECT:{
												   insertpair.second.ptr = engine->CreateScriptObjectCopy(newvalue, objtype_value);
												   break; }
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
													  switch(primitiveid_value){
													  case aatc_PRIMITIVE_TYPE::INT8:{insertpair.second.i8 = *((aatc_type_int8*)newvalue); break; }
													  case aatc_PRIMITIVE_TYPE::INT16:{insertpair.second.i16 = *((aatc_type_int16*)newvalue); break; }
													  case aatc_PRIMITIVE_TYPE::INT32:{insertpair.second.i32 = *((aatc_type_int32*)newvalue); break; }
													  case aatc_PRIMITIVE_TYPE::INT64:{insertpair.second.i64 = *((aatc_type_int64*)newvalue); break; }
													  case aatc_PRIMITIVE_TYPE::UINT8:{insertpair.second.ui8 = *((aatc_type_uint8*)newvalue); break; }
													  case aatc_PRIMITIVE_TYPE::UINT16:{insertpair.second.ui16 = *((aatc_type_uint16*)newvalue); break; }
													  case aatc_PRIMITIVE_TYPE::UINT32:{insertpair.second.ui32 = *((aatc_type_uint32*)newvalue); break; }
													  case aatc_PRIMITIVE_TYPE::UINT64:{insertpair.second.ui64 = *((aatc_type_uint64*)newvalue); break; }
													  case aatc_PRIMITIVE_TYPE::FLOAT32:{insertpair.second.f32 = *((aatc_type_float32*)newvalue); break; }
													  case aatc_PRIMITIVE_TYPE::FLOAT64:{insertpair.second.f64 = *((aatc_type_float64*)newvalue); break; }
													  };
													  break; }
			};

			T_container::insert(insertpair);
		}
	}

	void Erase_value(void* value){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
			if(need_errorcheck_missing_functions){
				aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::ERASE_VALUE, objtype_container->GetName(), objtype_key->GetName(), "erase")
			}
		#endif
		
		//if(datahandlingid_key == aatc_DATAHANDLINGTYPE::HANDLE){
		//	if(!value){
		//		return;
		//	}
		//}

		aatc_primunion findkey;

		BuildPrimunion(findkey, value, datahandlingid_key, primitiveid_key);
		//if(datahandlingid_key == aatc_DATAHANDLINGTYPE::STRING){ findkey.ptr = value; }

		T_container::iterator it = T_container::find(findkey);

		if(it != T_container::end()){
			aatc_primunion old_key;
			aatc_primunion old_value;

			if(datahandlingid_key != aatc_DATAHANDLINGTYPE::PRIMITIVE){ old_key.ptr = (*it).first.ptr; }
			if(datahandlingid_value != aatc_DATAHANDLINGTYPE::PRIMITIVE){ old_value.ptr = (*it).second.ptr; }

			T_container::erase(it);

			switch(datahandlingid_key){
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{break; }
			case aatc_DATAHANDLINGTYPE::STRING:{
												   //delete ((aatc_type_string*)old_key.ptr);
												   engine->ReleaseScriptObject(old_key.ptr, objtype_key);
												   break; }
			default:{
												   engine->ReleaseScriptObject(old_key.ptr, objtype_key);
												   break; }
			};
			switch(datahandlingid_value){
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{break; }
			case aatc_DATAHANDLINGTYPE::STRING:{
												   //delete ((aatc_type_string*)old_value.ptr);
												   engine->ReleaseScriptObject(old_value.ptr, objtype_value);
												   break; }
			default:{
						engine->ReleaseScriptObject(old_value.ptr, objtype_value);
						break; }
			};
		}
	}

	bool Contains_native(void* value){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
			if(need_errorcheck_missing_functions){
				aatc_errorcheck_container_missingfunctions_operation_retnull(aatc_CONTAINER_OPERATION::CONTAINS_NATIVE, objtype_container->GetName(), objtype_key->GetName(), "contains")
			}
		#endif

		//if(datahandlingid_key == aatc_DATAHANDLINGTYPE::HANDLE){
		//	if(!value){
		//		return 0;
		//	}
		//}

		aatc_primunion findkey;
		BuildPrimunion(findkey, value, datahandlingid_key, primitiveid_key);
		//if(datahandlingid_key == aatc_DATAHANDLINGTYPE::STRING){ findkey.ptr = value; }

		T_container::const_iterator it = T_container::find(findkey);
		return it != T_container::end();
	}

	const void* Find(void* value){
		bool success;
		return Find(value, success);
	}
	const void* Find(void* value, bool& success){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
			if(need_errorcheck_missing_functions){
				aatc_errorcheck_container_missingfunctions_operation_noret(aatc_CONTAINER_OPERATION::FIND, objtype_container->GetName(), objtype_key->GetName(), "find")
					return &aatc_primunion_defaultvalue.ptr;
				}
			}
		#endif

		//if(datahandlingid_key == aatc_DATAHANDLINGTYPE::HANDLE){
		//	if(!value){
		//		success = 0;
		//		return nullptr;
		//	}
		//}

		aatc_primunion findkey;
		BuildPrimunion(findkey, value, datahandlingid_key, primitiveid_key);
		//if(datahandlingid_key == aatc_DATAHANDLINGTYPE::STRING){ findkey.ptr = value; }

		T_container::const_iterator it = T_container::find(findkey);
		if(it == T_container::end()){
			success = 0;
			switch(datahandlingid_value){
			case aatc_DATAHANDLINGTYPE::HANDLE:{return &(aatc_primunion_defaultvalue.ptr); }
			case aatc_DATAHANDLINGTYPE::OBJECT:{return aatc_primunion_defaultvalue.ptr; }
			case aatc_DATAHANDLINGTYPE::STRING:{return aatc_primunion_defaultvalue.ptr; }
				case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
					switch(primitiveid_value){
						case aatc_PRIMITIVE_TYPE::INT8:{return &(aatc_primunion_defaultvalue.i8); }
						case aatc_PRIMITIVE_TYPE::INT16:{return &(aatc_primunion_defaultvalue.i16); }
						case aatc_PRIMITIVE_TYPE::INT32:{return &(aatc_primunion_defaultvalue.i32); }
						case aatc_PRIMITIVE_TYPE::INT64:{return &(aatc_primunion_defaultvalue.i64); }
						case aatc_PRIMITIVE_TYPE::UINT8:{return &(aatc_primunion_defaultvalue.ui8); }
						case aatc_PRIMITIVE_TYPE::UINT16:{return &(aatc_primunion_defaultvalue.ui16); }
						case aatc_PRIMITIVE_TYPE::UINT32:{return &(aatc_primunion_defaultvalue.ui32); }
						case aatc_PRIMITIVE_TYPE::UINT64:{return &(aatc_primunion_defaultvalue.ui64); }
						case aatc_PRIMITIVE_TYPE::FLOAT32:{return &(aatc_primunion_defaultvalue.f32); }
						case aatc_PRIMITIVE_TYPE::FLOAT64:{return &(aatc_primunion_defaultvalue.f64); }
					};
				break; }
			};
			return &aatc_primunion_defaultvalue.ptr;
		} else{
			success = 1;
			switch(datahandlingid_value){
				case aatc_DATAHANDLINGTYPE::HANDLE:{return &((*it).second.ptr); }
				case aatc_DATAHANDLINGTYPE::OBJECT:{return (*it).second.ptr; }
				case aatc_DATAHANDLINGTYPE::STRING:{return (*it).second.ptr; }
				case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
					switch(primitiveid_value){
						case aatc_PRIMITIVE_TYPE::INT8:{return &((*it).second.i8); }
						case aatc_PRIMITIVE_TYPE::INT16:{return &((*it).second.i16); }
						case aatc_PRIMITIVE_TYPE::INT32:{return &((*it).second.i32); }
						case aatc_PRIMITIVE_TYPE::INT64:{return &((*it).second.i64); }
						case aatc_PRIMITIVE_TYPE::UINT8:{return &((*it).second.ui8); }
						case aatc_PRIMITIVE_TYPE::UINT16:{return &((*it).second.ui16); }
						case aatc_PRIMITIVE_TYPE::UINT32:{return &((*it).second.ui32); }
						case aatc_PRIMITIVE_TYPE::UINT64:{return &((*it).second.ui64); }
						case aatc_PRIMITIVE_TYPE::FLOAT32:{return &((*it).second.f32); }
						case aatc_PRIMITIVE_TYPE::FLOAT64:{return &((*it).second.f64); }
					};
				break; }
			};
			return &aatc_primunion_defaultvalue.ptr;
		}
	}
};


template<class T_container> void aatc_container_shared_map_template_Register(asIScriptEngine* engine, const char* n_container){
	int r = 0;

	char n_container_T[1000];
	sprintf_s(n_container_T,1000, "%s<T_key,T_value>", n_container);

	char n_container_class_T[1000];
	sprintf_s(n_container_class_T, 1000, "%s<class T_key,class T_value>", n_container);

	char textbuf[1000];

	r = engine->RegisterObjectType(n_container_class_T, 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s@ f(int&in)", n_container_T);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_FACTORY, textbuf, asFUNCTIONPR(T_container::Factory, (asIObjectType*), T_container*), asCALL_CDECL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s@ f(int&in,const %s &in)", n_container_T, n_container_T);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_FACTORY, textbuf, asFUNCTIONPR(T_container::Factory_copy, (asIObjectType*, const T_container&), T_container*), asCALL_CDECL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s& opAssign(const %s &in)", n_container_T, n_container_T);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, operator=), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s& %s(%s &in)", n_container_T, aatc_name_script_container_method_swap, n_container_T);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Swap), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_ADDREF, "void f()", asMETHOD(T_container, refcount_Add), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_RELEASE, "void f()", asMETHOD(T_container, refcount_Release), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_SETGCFLAG, "void f()", asMETHOD(aatc_refcounted_GC, SetGCFlag), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(aatc_refcounted_GC, GetGCFlag), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(aatc_refcounted_GC, GetRefCount), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_ENUMREFS, "void f(int&in)", asMETHODPR(T_container, EnumReferences, (asIScriptEngine*), void), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHODPR(T_container, ReleaseAllReferences, (asIScriptEngine*), void), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(aatc_templatecallback_container), asCALL_CDECL); assert(r >= 0);


	sprintf_s(textbuf, 1000, "void %s(bool)", aatc_name_script_container_method_set_directcomp);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, SetDirectcomp), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "void %s()", aatc_name_script_container_method_clear);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Clear), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "bool %s()", aatc_name_script_container_method_empty);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Empty), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s %s()", aatc_name_script_sizetype, aatc_name_script_container_method_size);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Size), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "void %s(const T_key&in,const T_value&in)", aatc_name_script_container_method_insert);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Insert), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "void %s(const T_key&in)", aatc_name_script_container_method_erase);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Erase_value), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "bool %s(const T_key&in)", aatc_name_script_container_method_contains);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Contains_native), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "T_value& %s(const T_key &in)", aatc_name_script_container_method_find);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHODPR(T_container, Find, (void*), const void*), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "T_value& %s(const T_key &in,bool &out)", aatc_name_script_container_method_find);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHODPR(T_container, Find, (void*, bool&), const void*), asCALL_THISCALL); assert(r >= 0);
}

template<typename T_out, typename T_host> T_out aatc_reghelp_construct_hosted_iterator_map_template(T_host cont){
	return T_out(&cont, 0);
}


/*!\brief Internal template monster

*/
template<class T_container> class aect_iterator_shared_map_template{
public:
	T_container* host;

	typename T_container::iterator it;
	typename T_container::iterator it_end;

	bool cont;
	bool firstt;
	//bool handlemode_key;
	//bool handlemode_value;

	aatc_DATAHANDLINGTYPE datahandlingid_key;
	aatc_DATAHANDLINGTYPE datahandlingid_value;
	aatc_PRIMITIVE_TYPE primitiveid_key;
	aatc_PRIMITIVE_TYPE primitiveid_value;

	aect_iterator_shared_map_template(){}
	aect_iterator_shared_map_template(void *ref, int typeId_target_container) :
		firstt(1)
	{
		host = (T_container*)(*(void**)ref);
		Init();
	}
	aect_iterator_shared_map_template(const aect_iterator_shared_map_template& other):
		host(other.host),
		it(other.it),
		it_end(other.it_end),
		cont(other.cont),
		firstt(other.firstt),
		datahandlingid_key(other.datahandlingid_key),
		datahandlingid_value(other.datahandlingid_value),
		primitiveid_key(other.primitiveid_key),
		primitiveid_value(other.primitiveid_value)
	{}
	aect_iterator_shared_map_template& operator=(const aect_iterator_shared_map_template& other){
		host = other.host;
		it = other.it;
		it_end = other.it_end;
		firstt = other.firstt;
		cont = other.cont;
		datahandlingid_key = other.datahandlingid_key;
		datahandlingid_value = other.datahandlingid_value;
		primitiveid_key = other.primitiveid_key;
		primitiveid_key = other.primitiveid_key;

		return *this;
	}

	static void static_constructor(asIObjectType* objtype_container, void *ref, int typeId, void *memory){
		new(memory)aect_iterator_shared_map_template(ref, typeId);
	}

	void Init(){
		if (host->empty()){
			cont = 0;
		}else{
			//handlemode_key = (host->datahandlingid_key == aatc_DATAHANDLINGTYPE::HANDLE);
			//handlemode_value = (host->datahandlingid_value == aatc_DATAHANDLINGTYPE::HANDLE);

			datahandlingid_key = host->datahandlingid_key;
			datahandlingid_value = host->datahandlingid_value;
			primitiveid_key = host->primitiveid_key;
			primitiveid_value = host->primitiveid_value;

			it = host->begin();
			it_end = host->end();
			cont = 1;
		}
	}

	//combine end check and continuation into one monster
	bool Next(){
		if (firstt){
			if (cont){//all is well
				firstt = 0;
				return 1;
			}else{//cont set to 0 in constructor because container is empty
				return 0;
			}
		}else{
			it++;
			//if (it == host->end()){
			if (it == it_end){
				return 0;
			}else{
				return 1;
			}
		}
	}

	const void* Current_key_get(){
		switch(datahandlingid_key){
		case aatc_DATAHANDLINGTYPE::HANDLE:{return &((*it).first.ptr); }//return pointer to handle
		case aatc_DATAHANDLINGTYPE::OBJECT:{return (*it).first.ptr; }//return copy of pointer to object
		case aatc_DATAHANDLINGTYPE::STRING:{return (*it).first.ptr; }//return copy of pointer to object
		case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
												  switch(primitiveid_key){
												  case aatc_PRIMITIVE_TYPE::INT8:{return &((*it).first.i8); }
												  case aatc_PRIMITIVE_TYPE::INT16:{return &((*it).first.i16); }
												  case aatc_PRIMITIVE_TYPE::INT32:{return &((*it).first.i32); }
												  case aatc_PRIMITIVE_TYPE::INT64:{return &((*it).first.i64); }
												  case aatc_PRIMITIVE_TYPE::UINT8:{return &((*it).first.ui8); }
												  case aatc_PRIMITIVE_TYPE::UINT16:{return &((*it).first.ui16); }
												  case aatc_PRIMITIVE_TYPE::UINT32:{return &((*it).first.ui32); }
												  case aatc_PRIMITIVE_TYPE::UINT64:{return &((*it).first.ui64); }
												  case aatc_PRIMITIVE_TYPE::FLOAT32:{return &((*it).first.f32); }
												  case aatc_PRIMITIVE_TYPE::FLOAT64:{return &((*it).first.f64); }
												  };
		}
		};
		return nullptr;//should never happen, stops compiler warning
	}

	const void* Current_value_get(){
		switch(datahandlingid_value){
		case aatc_DATAHANDLINGTYPE::HANDLE:{return &((*it).second.ptr); }//return pointer to handle
		case aatc_DATAHANDLINGTYPE::OBJECT:{return (*it).second.ptr; }//return copy of pointer to object
		case aatc_DATAHANDLINGTYPE::STRING:{return (*it).second.ptr; }//return copy of pointer to object
		case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
												  switch(primitiveid_key){
												  case aatc_PRIMITIVE_TYPE::INT8:{return &((*it).second.i8); }
												  case aatc_PRIMITIVE_TYPE::INT16:{return &((*it).second.i16); }
												  case aatc_PRIMITIVE_TYPE::INT32:{return &((*it).second.i32); }
												  case aatc_PRIMITIVE_TYPE::INT64:{return &((*it).second.i64); }
												  case aatc_PRIMITIVE_TYPE::UINT8:{return &((*it).second.ui8); }
												  case aatc_PRIMITIVE_TYPE::UINT16:{return &((*it).second.ui16); }
												  case aatc_PRIMITIVE_TYPE::UINT32:{return &((*it).second.ui32); }
												  case aatc_PRIMITIVE_TYPE::UINT64:{return &((*it).second.ui64); }
												  case aatc_PRIMITIVE_TYPE::FLOAT32:{return &((*it).second.f32); }
												  case aatc_PRIMITIVE_TYPE::FLOAT64:{return &((*it).second.f64); }
												  };
		}
		};
		return nullptr;//should never happen, stops compiler warning
	}
	void Current_value_set(void* value){
		switch(datahandlingid_value){
		case aatc_DATAHANDLINGTYPE::HANDLE:{
			void** it_inner = &((*it).second.ptr);//convenience

			if(*it_inner){
				host->engine->ReleaseScriptObject(*it_inner, host->objtype_value);
			}
			if(value){
				*it_inner = host->StoreHandle2(value, host->objtype_value);
			} else{
				*it_inner = nullptr;
			}
		break; }
		case aatc_DATAHANDLINGTYPE::OBJECT:
		case aatc_DATAHANDLINGTYPE::STRING:{
			//auto& it_inner = (*it).second.ptr;//convenience
			//void** it_inner = (void**)(*it).second.ptr;//convenience
			void** it_inner = &((*it).second.ptr);//convenience
			host->engine->ReleaseScriptObject(*it_inner, host->objtype_value);
			*it_inner = host->engine->CreateScriptObjectCopy(value, host->objtype_value);
		break; }
		case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
			switch(primitiveid_value){
				case aatc_PRIMITIVE_TYPE::INT8:{((*it).second.i8) = *((aatc_type_int8*)value); break; }
				case aatc_PRIMITIVE_TYPE::INT16:{((*it).second.i16) = *((aatc_type_int16*)value); break; }
				case aatc_PRIMITIVE_TYPE::INT32:{((*it).second.i32) = *((aatc_type_int32*)value); break; }
				case aatc_PRIMITIVE_TYPE::INT64:{((*it).second.i64) = *((aatc_type_int64*)value); break; }

				case aatc_PRIMITIVE_TYPE::UINT8:{((*it).second.ui8) = *((aatc_type_uint8*)value); break; }
				case aatc_PRIMITIVE_TYPE::UINT16:{((*it).second.ui16) = *((aatc_type_uint16*)value); break; }
				case aatc_PRIMITIVE_TYPE::UINT32:{((*it).second.ui32) = *((aatc_type_uint32*)value); break; }
				case aatc_PRIMITIVE_TYPE::UINT64:{((*it).second.ui64) = *((aatc_type_uint64*)value); break; }

				case aatc_PRIMITIVE_TYPE::FLOAT32:{((*it).second.f32) = *((aatc_type_float32*)value); break; }
				case aatc_PRIMITIVE_TYPE::FLOAT64:{((*it).second.f64) = *((aatc_type_float64*)value); break; }
			};
		break; }
		};
	}


	const void* Current_key_const(){
		switch(datahandlingid_key){
			case aatc_DATAHANDLINGTYPE::HANDLE:{return &((*it).first.ptr); }//return pointer to handle
			case aatc_DATAHANDLINGTYPE::OBJECT:{return (*it).first.ptr; }//return copy of pointer to object
			case aatc_DATAHANDLINGTYPE::STRING:{return (*it).first.ptr; }//return copy of pointer to object
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
				switch(primitiveid_key){
					case aatc_PRIMITIVE_TYPE::INT8:{return &((*it).first.i8); }
					case aatc_PRIMITIVE_TYPE::INT16:{return &((*it).first.i16); }
					case aatc_PRIMITIVE_TYPE::INT32:{return &((*it).first.i32); }
					case aatc_PRIMITIVE_TYPE::INT64:{return &((*it).first.i64); }
					case aatc_PRIMITIVE_TYPE::UINT8:{return &((*it).first.ui8); }
					case aatc_PRIMITIVE_TYPE::UINT16:{return &((*it).first.ui16); }
					case aatc_PRIMITIVE_TYPE::UINT32:{return &((*it).first.ui32); }
					case aatc_PRIMITIVE_TYPE::UINT64:{return &((*it).first.ui64); }
					case aatc_PRIMITIVE_TYPE::FLOAT32:{return &((*it).first.f32); }
					case aatc_PRIMITIVE_TYPE::FLOAT64:{return &((*it).first.f64); }
				};
			}
		};
		return nullptr;//should never happen, stops compiler warning
	}

	void* Current_value(){
		switch(datahandlingid_value){
			case aatc_DATAHANDLINGTYPE::HANDLE:{return &((*it).second.ptr); }//return pointer to handle
			case aatc_DATAHANDLINGTYPE::OBJECT:{return (*it).second.ptr; }//return copy of pointer to object
			case aatc_DATAHANDLINGTYPE::STRING:{return (*it).second.ptr; }//return copy of pointer to object
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
				switch(primitiveid_value){
					case aatc_PRIMITIVE_TYPE::INT8:{return &((*it).second.i8); }
					case aatc_PRIMITIVE_TYPE::INT16:{return &((*it).second.i16); }
					case aatc_PRIMITIVE_TYPE::INT32:{return &((*it).second.i32); }
					case aatc_PRIMITIVE_TYPE::INT64:{return &((*it).second.i64); }
					case aatc_PRIMITIVE_TYPE::UINT8:{return &((*it).second.ui8); }
					case aatc_PRIMITIVE_TYPE::UINT16:{return &((*it).second.ui16); }
					case aatc_PRIMITIVE_TYPE::UINT32:{return &((*it).second.ui32); }
					case aatc_PRIMITIVE_TYPE::UINT64:{return &((*it).second.ui64); }
					case aatc_PRIMITIVE_TYPE::FLOAT32:{return &((*it).second.f32); }
					case aatc_PRIMITIVE_TYPE::FLOAT64:{return &((*it).second.f64); }
				};
			}
		};
		return nullptr;//should never happen, stops compiler warning
	}

	static void Register(asIScriptEngine* engine, const char* n_iterator, const char* n_container_T){
		int r = 0;
		char textbuf[1000];

		char n_iterator_T[1000];
		sprintf_s(n_iterator_T, 1000, "%s<T_key,T_value>", n_iterator);

		char n_iterator_class_T[1000];
		sprintf_s(n_iterator_class_T, 1000, "%s<class T_key,class T_value>", n_iterator);

		r = engine->RegisterObjectType(n_iterator_class_T, sizeof(aect_iterator_shared_map_template), asOBJ_VALUE | asOBJ_TEMPLATE | asGetTypeTraits<aect_iterator_shared_map_template>()); assert(r >= 0);

		//the default constructor must be registered, but you should never use it in script
		r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in)", asFunctionPtr(aatc_reghelp_constructor_template_default<aect_iterator_shared_map_template>), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in,?&in)", asFunctionPtr(static_constructor), asCALL_CDECL_OBJLAST); assert(r >= 0);
		//sprintf(textbuf, "void f(int&in,%s@)", n_container_T);
		//r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(static_constructor), asCALL_CDECL_OBJLAST); assert(r >= 0);

		//sprintf_s(textbuf, 1000, "void f(const %s &in)", n_iterator_T);
		//r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(aatc_reghelp_constructor_copy<aect_iterator_shared_map_template, aect_iterator_shared_map_template>), asCALL_CDECL_OBJLAST); assert(r >= 0);

		r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(aatc_reghelp_generic_destructor<aect_iterator_shared_map_template>), asCALL_CDECL_OBJLAST); assert(r >= 0);

		//r = engine->RegisterObjectMethod(n_iterator_T, "const T_key& current_key()", asMETHOD(aect_iterator_shared_map_template, Current_key_const), asCALL_THISCALL); assert(r >= 0);
		//r = engine->RegisterObjectMethod(n_iterator_T, "T_value& current_value()", asMETHOD(aect_iterator_shared_map_template, Current_value), asCALL_THISCALL); assert(r >= 0);

		sprintf_s(textbuf, 1000, "const T_key& %s()", aatc_name_script_iterator_access_function_key);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_map_template, Current_key_const), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "T_value& %s()", aatc_name_script_iterator_access_function_value);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_map_template, Current_value), asCALL_THISCALL); assert(r >= 0);

		sprintf_s(textbuf, 1000, "const T_key& get_%s()", aatc_name_script_iterator_access_property_key);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_map_template, Current_key_get), asCALL_THISCALL); assert(r >= 0);

		sprintf_s(textbuf, 1000, "T_value& get_%s()", aatc_name_script_iterator_access_property_value);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_map_template, Current_value_get), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void set_%s(const T_value &in)", aatc_name_script_iterator_access_property_value);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_map_template, Current_value_set), asCALL_THISCALL); assert(r >= 0);

		r = engine->RegisterObjectMethod(n_iterator_T, "bool next()", asMETHOD(aect_iterator_shared_map_template, Next), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(n_iterator_T, "bool opPreInc()", asMETHOD(aect_iterator_shared_map_template, Next), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(n_iterator_T, "bool opPostInc()", asMETHOD(aect_iterator_shared_map_template, Next), asCALL_THISCALL); assert(r >= 0);

		sprintf_s(textbuf, 1000, "%s& opAssign(const %s &in)", n_iterator_T, n_iterator_T);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_map_template, operator=), asCALL_THISCALL); assert(r >= 0);
	}
};



END_AS_NAMESPACE

#endif