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
#include "aatc_enginestorage.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace common {



		std_Spinlock::std_Spinlock() : state(1) {}
		void std_Spinlock::lock(){while (state.exchange(0, std::memory_order_acquire) == 0);}
		void std_Spinlock::unlock(){state.store(1, std::memory_order_release);}






		primunion primunion_defaultvalue = primunion();






		//refcounted and gc basetype
		//refcounted and gc basetype
		aatc_refcounted::aatc_refcounted() :
			refcount(1)
		{}
		aatc_refcounted::~aatc_refcounted(){}
		void aatc_refcounted::refcount_Add(){
			asAtomicInc(refcount);
		}
		void aatc_refcounted::refcount_Release(){
			if (asAtomicDec(refcount) == 0){
				delete this;
			}
		}

		aatc_refcounted_GC::aatc_refcounted_GC():
			refCount(1),
			gcFlag(0)
		{}
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
			enginestorage::engine_level_storage* ss = (enginestorage::engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);

			ss->Clean();
			delete ss;
		}

		bool aatc_templatecallback_1tp(asIObjectType *ot, bool &dontGarbageCollect){
			return aatc_templatecallback_typeid(ot, ot->GetSubTypeId(), dontGarbageCollect);
		}
		bool aatc_templatecallback_map(asIObjectType *ot, bool &dontGarbageCollect){
			int typeId_key = ot->GetSubTypeId(0);
			int typeId_value = ot->GetSubTypeId(1);

			bool dont_gc_key = 0;
			bool dont_gc_value = 0;
			bool result_key = aatc_templatecallback_typeid(ot, typeId_key, dont_gc_key);
			bool result_value = aatc_templatecallback_typeid(ot, typeId_value, dont_gc_value);

			dontGarbageCollect = dont_gc_key && dont_gc_value;

			return result_key && result_value;
		}
		bool aatc_templatecallback_typeid(asIObjectType *ot, int typeId, bool &dontGarbageCollect){
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
			enginestorage::engine_level_storage* els = (enginestorage::engine_level_storage*)asGetActiveContext()->GetEngine()->GetUserData(aatc_engine_userdata_id);
			asIScriptContext* c = els->contextcache_Get();
				Prepare(c);
				Execute(c);
			els->contextcache_Return(c);
		}

		//void aatc_script_Funcpointer::ReleaseRef(){
		//	ref.Set(NULL, NULL);
		//}


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

		void aatc_errorprint_iterator_container_modified(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_EXCEPTIONS
				asGetActiveContext()->SetException(aatc_errormessage_iterator_container_modified);
			#endif
		}
		void aatc_errorprint_container_iterator_invalid(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_EXCEPTIONS
				asGetActiveContext()->SetException(aatc_errormessage_container_iterator_invalid);
			#endif
		}



		DATAHANDLINGTYPE aatc_Determine_Datahandlingtype(asIScriptEngine* engine,aatc_type_uint32 astypeid){
			if(astypeid == engine->GetStringFactoryReturnTypeId()){
				return DATAHANDLINGTYPE::STRING;
			}
			if(astypeid & asTYPEID_MASK_OBJECT){
				if(astypeid & asTYPEID_OBJHANDLE){
					return DATAHANDLINGTYPE::HANDLE;
				} else{
					return DATAHANDLINGTYPE::OBJECT;
				}
			} else{
				return DATAHANDLINGTYPE::PRIMITIVE;
			}
		}
		PRIMITIVE_TYPE aatc_Determine_Primitivetype(aatc_type_uint32 astypeid){
			switch(astypeid){
				case asTYPEID_BOOL:{return PRIMITIVE_TYPE::INT8; }
				case asTYPEID_INT8:{return PRIMITIVE_TYPE::INT8; }
				case asTYPEID_INT16:{return PRIMITIVE_TYPE::INT16; }
				case asTYPEID_INT32:{return PRIMITIVE_TYPE::INT32; }
				case asTYPEID_INT64:{return PRIMITIVE_TYPE::INT64; }
				case asTYPEID_UINT8:{return PRIMITIVE_TYPE::UINT8; }
				case asTYPEID_UINT16:{return PRIMITIVE_TYPE::UINT16; }
				case asTYPEID_UINT32:{return PRIMITIVE_TYPE::UINT32; }
				case asTYPEID_UINT64:{return PRIMITIVE_TYPE::UINT64; }
				case asTYPEID_FLOAT:{return PRIMITIVE_TYPE::FLOAT32; }
				case asTYPEID_DOUBLE:{return PRIMITIVE_TYPE::FLOAT64; }
				default: {return PRIMITIVE_TYPE::INT8; }
			};
		}

		//container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions_base(int CONTAINER_ID, template_specific_storage* tss){
		//	switch(CONTAINER_ID){
		//		case aatc_CONTAINERTYPE::VECTOR: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::VECTOR>(tss);
		//		case aatc_CONTAINERTYPE::LIST: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::LIST>(tss);
		//		case aatc_CONTAINERTYPE::SET: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::SET>(tss);
		//		case aatc_CONTAINERTYPE::UNORDERED_SET: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::UNORDERED_SET>(tss);
		//		case aatc_CONTAINERTYPE::MAP: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::MAP>(tss);
		//		case aatc_CONTAINERTYPE::UNORDERED_MAP: return aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::UNORDERED_MAP>(tss);
		//	};
		//	return 0;
		//}



		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<bool>(bool* input){ return asTYPEID_BOOL; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_int8>(aatc_type_int8* input){ return asTYPEID_INT8; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_int16>(aatc_type_int16* input){ return asTYPEID_INT16; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_int32>(aatc_type_int32* input){ return asTYPEID_INT32; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_int64>(aatc_type_int64* input){ return asTYPEID_INT64; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_uint8>(aatc_type_uint8* input){ return asTYPEID_UINT8; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_uint16>(aatc_type_uint16* input){ return asTYPEID_UINT16; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_uint32>(aatc_type_uint32* input){ return asTYPEID_UINT32; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_uint64>(aatc_type_uint64* input){ return asTYPEID_UINT64; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_float32>(aatc_type_float32* input){ return asTYPEID_FLOAT; }
		//template<> aatc_type_astypeid aatc_get_primitive_astypeid_by_cpptype<aatc_type_float64>(aatc_type_float64* input){ return asTYPEID_DOUBLE; }


		iterator_base::iterator_base():
			firstt(1)
		{}
		iterator_base::iterator_base(const iterator_base& other) :
			firstt(other.firstt),
			cont(other.cont)
		{}



		void* primunion::Get_Ptr_To_Primitive_Type(PRIMITIVE_TYPE primtype){
			switch(primtype){
				case PRIMITIVE_TYPE::INT8:{return &i8; }
				case PRIMITIVE_TYPE::INT16:{return &i16; }
				case PRIMITIVE_TYPE::INT32:{return &i32; }
				case PRIMITIVE_TYPE::INT64:{return &i64; }
				case PRIMITIVE_TYPE::UINT8:{return &ui8; }
				case PRIMITIVE_TYPE::UINT16:{return &ui16; }
				case PRIMITIVE_TYPE::UINT32:{return &ui32; }
				case PRIMITIVE_TYPE::UINT64:{return &ui64; }
				case PRIMITIVE_TYPE::FLOAT32:{return &f32; }
				case PRIMITIVE_TYPE::FLOAT64:{return &f64; }
			};
			return nullptr;
		}
		const void* primunion::Get_Ptr_To_Primitive_Type_const(PRIMITIVE_TYPE primtype)const{
			return const_cast<primunion*>(this)->Get_Ptr_To_Primitive_Type(primtype);
			//return const_cast<void*>(Get_Ptr_To_Primitive_Type(primtype));
		}

		void primunion::Init(){
			ui32 = 0;
			ptr = nullptr;
		}





		RegistrationState::RegistrationState(asIScriptEngine* engine):
			engine(engine),
			error(0)
		{}



	};//namespace common
};//namespace aatc
END_AS_NAMESPACE