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


#ifndef _includedh_aatc_shared_1t_generic
#define _includedh_aatc_shared_1t_generic



#include "aatc_common.hpp"
#include "aatc_container_shared.hpp"

//#include "cm/core/print.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace shared {
			namespace templated {



				//doxygen skip
				#ifndef DOXYGEN_SHOULD_SKIP_THIS

				//we need a wrapper around the base container so that we can
				//feed the constructor a fancy functor to handle internal functions that require script functions to be called
				namespace base_container_wrapper {
					template<class T_container> class Basic : public T_container {
					public:
						Basic(asIScriptEngine* engine, containerfunctor::Settings* settings) {}
					};

					template<class T_container, class T_param1> class Singleparam : public T_container {
					public:
						Singleparam(asIScriptEngine* _engine, containerfunctor::Settings* settings) :
							T_container(T_param1(_engine, settings))
						{}
					};

					template<class T_container, class T_param1, class T_param2, std::size_t default_bucket_count> class Hashed : public T_container {
					public:
						Hashed(asIScriptEngine* _engine, containerfunctor::Settings* settings) :
							T_container(default_bucket_count, T_param1(_engine, settings), T_param2(_engine, settings))
						{}
					};
				};

				#endif
				//doxygen skip



				/*!\brief Internal template monster

				*/
				template<class T_container, int T_CONTAINERTYPEID, class bcw = base_container_wrapper::Basic<T_container>> class Containerbase :
					public aatc::container::shared::container_basicbase,
					public bcw,
					public aatc::common::aatc_refcounted_GC,
					public aatc::container::shared::containerfunctor::Settings
				{
				public:
					typename typedef T_container::iterator iteratortype;
					typename typedef T_container T_actual_container;

					//typename typedef std::conditional<NEED_COMPHELPER, aatc_bcw_comphelper<T_container>, aatc_bcw_basic<T_container>>::type bct;
					//typename typedef bcw bct;

					asIObjectType* objtype_container;
					asIObjectType* objtype_content;
					aatc_type_astypeid astypeid_container;
					aatc_type_astypeid astypeid_content;
					asDWORD typeflags_container;
					asDWORD typeflags_content;

					bool handlemode_needref;

					aatc::common::engine_level_storage* els;

					aatc::common::container_operations_bitmask_type missing_functions;

					bool directcomp_forced;

					Containerbase(asIScriptEngine* _engine, asIObjectType* _objtype) :
						aatc::container::shared::container_basicbase(_engine),
						bcw(_engine, this),
						aatc_refcounted_GC(),
						objtype_container(_objtype),
						directcomp_forced(0)
					{
						handlemode_directcomp = aatc_CONFIG_DEFAULT_HANDLEMODE_DIRECTCOMP;

						objtype_content = objtype_container->GetSubType();

						astypeid_container = objtype_container->GetTypeId();
						astypeid_content = objtype_container->GetSubTypeId();

						typeflags_container = objtype_container->GetFlags();
						typeflags_content = objtype_content->GetFlags();

						//problem_nofunc_eq_or_cmp = 0;
						//problem_nofunc_cmp = 0;

						els = aatc::common::aatc_Get_ELS(engine);
						aatc::common::containertype_specific_storage* ctss = els->GetContainerTypeSpecificStorage(T_CONTAINERTYPEID);

						if (astypeid_content & asTYPEID_OBJHANDLE) {
							handlemode = 1;
							handlemode_needref = 1;
							if (typeflags_content & asOBJ_NOCOUNT) {
								handlemode_needref = 0;
							}
						} else {
							handlemode = 0;
							handlemode_directcomp = 0;
						}

						aatc::common::template_specific_storage* tss = ctss->GetTemplateSpecificStorage(astypeid_content);
						func_equals = tss->func_equals;
						func_cmp = tss->func_cmp;
						func_hash = tss->func_hash;

						missing_functions = tss->missing_functions;

						#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE
								if (missing_functions) {
									bool bad = 1;

						#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE_USE_DIRECTCOMP
									if (handlemode) {//force handlemode directcomp if anything is missing
										handlemode_directcomp = 1;
										directcomp_forced = 1;
										bad = 0;
									}
						#endif

									if (bad) {
										char msg[1000];
										sprintf_s(msg, 1000, "Type '%s' has missing methods required for container '%s'.", objtype_content->GetName(), objtype_container->GetName());
										asGetActiveContext()->SetException(msg);
									}
								}
						#endif

						engine->NotifyGarbageCollectorOfNewObject(this, objtype_container);
					}
					virtual ~Containerbase() {
						Clear();
					}

					void operator=(const Containerbase& other) {
						safety_iteratorversion_Increment();

						Clear();//will delete script objects or decrement handles

						T_container::operator=(other);

						if (handlemode) {//increment refs for newly created handles
							for (auto it = T_container::begin(); it != T_container::end(); it++) {
								engine->AddRefScriptObject(*it, objtype_content);
							}
						} else {//value objects must be copied
								/*
								TODO: Safety for associative containers (set,unordered_set).

								Copying script objects will call script functions and custom
								script functions might not perfectly copy whatever is keeping
								the associative container sorted, thus sorting might explode.

								Implement some flag to tell the container that your custom
								script copy function is indeed safe. If the flag is set, use
								simple copy like this, otherwise copy all objects first and
								insert to the new container after copy.
								*/

							T_container::iterator it = T_container::begin();
							T_container::iterator it_other = const_cast<aatc_container_shared_1tp_template&>(other).begin();
							T_container::iterator it_end = T_container::end();

							for (; it != it_end;) {
								void*& iii = const_cast<void*&>(*it);
								void*& iii_other = const_cast<void*&>(*it_other);

								iii = engine->CreateScriptObjectCopy(iii_other, objtype_content);

								it++;
								it_other++;
							}
						}
					}
					void swap(Containerbase& other) {
						T_container::swap(static_cast<T_container&>(other));
						safety_iteratorversion_Increment();
						other.safety_iteratorversion_Increment();
					}
					//aatc_container_shared_1tp_template(const aatc_container_shared_1tp_template& other){
					//	(*this) = other;
					//}

					void SetDirectcomp(bool ss) {
						if (directcomp_forced) { return; }
						handlemode_directcomp = ss;
					}

					//handle input is void* that points at handle which is a void*
					//handle output is void* that points at handle which is a void*

					void StoreHandle(void** target, void* ptr_to_handle) {
						*target = *(void**)ptr_to_handle;
						//if (handlemode_needref){
						engine->AddRefScriptObject(*target, objtype_content);
						//}
					}
					void* StoreHandle2(void* ptr_to_handle) {
						void* result = *(void**)ptr_to_handle;
						//if(handlemode_needref){
						engine->AddRefScriptObject(result, objtype_content);
						//}
						return result;
					}
					void ReleaseHandle(void* handle) {
						//if (handlemode_needref){
						engine->ReleaseScriptObject(handle, objtype_content);
						//}
					}

					aatc_type_sizetype Count(void* value) {
						if (handlemode_directcomp) {
							return (aatc_type_sizetype)(std::count(T_container::begin(), T_container::end(), *(void**)value));
						} else {
							aatc_errorcheck_container_missingfunctions_operation_retnull(aatc_CONTAINER_OPERATION::COUNT, objtype_container->GetName(), objtype_content->GetName(), "count")

								if (handlemode) { value = *(void**)value; }

							asIScriptContext* cc = els->contextcache_Get();

							aatc_type_sizetype count = 0;
							T_container::iterator it = T_container::begin();
							T_container::iterator itend = T_container::end();

							asIScriptFunction* func = func_cmp;
							if (func_equals) { func = func_equals; }

							if (func_equals) {
								for (; it != itend; it++) {
									cc->Prepare(func);
									cc->SetObject(value);
									cc->SetArgObject(0, *it);
									cc->Execute();
									count += (cc->GetReturnByte());
								}
							} else {//func cmp
								for (; it != itend; it++) {
									cc->Prepare(func);
									cc->SetObject(value);
									cc->SetArgObject(0, *it);
									cc->Execute();
									count += (cc->GetReturnDWord() == 0);
								}
							}
							els->contextcache_Return(cc);

							return count;
						}
					};
					bool Contains_generic(void* value) { return Count(value) > 0; }


					void Clear() {
						safety_iteratorversion_Increment();

						T_container::iterator it = T_container::begin();
						T_container::iterator itend = T_container::end();
						if (handlemode) {
							for (; it != itend; it++) {
								ReleaseHandle(*it);
							}
						} else {
							for (; it != itend; it++) {
								engine->ReleaseScriptObject(*it, objtype_content);
							}
						}
						T_container::clear();
					}
					bool Empty() { return T_container::empty(); }
					aatc_type_sizetype Size() { return (aatc_type_sizetype)(T_container::size()); }

					void EnumReferences(asIScriptEngine* engine) {
						if (astypeid_content & asTYPEID_MASK_OBJECT) {//dont do this for primitives
							T_container::iterator it = T_container::begin();
							T_container::iterator itend = T_container::end();
							for (; it != itend; it++) {
								engine->GCEnumCallback(*it);
							}
						}
					}
					void ReleaseAllReferences(asIScriptEngine* engine) {
						Clear();
					}










				};








			};//namespace templated
		};//namespace shared
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif