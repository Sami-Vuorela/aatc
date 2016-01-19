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


#ifndef _includedh_aatc_container_shared_template
#define _includedh_aatc_container_shared_template



#include "aatc_common.hpp"
#include "aatc_container_shared.hpp"



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
				template<class T_container, int T_CONTAINERTYPEID, typename T_container_tags = aatc::container::shared::tagbase, class bcw = base_container_wrapper::Basic<T_container>> class Containerbase :
					public aatc::container::shared::container_basicbase,
					public aatc::common::aatc_refcounted_GC,
					public aatc::container::shared::containerfunctor::Settings
				{
				public:
					typename typedef T_container::iterator iteratortype;
					typename typedef T_container T_actual_container;

					typename bcw container;

					aatc::common::engine_level_storage* els;

					asIObjectType* objtype_container;
					asIObjectType* objtype_content;
					aatc_type_astypeid astypeid_container;
					aatc_type_astypeid astypeid_content;
					asDWORD typeflags_container;
					asDWORD typeflags_content;

					bool handlemode_needref;

					aatc::common::container_operations_bitmask_type missing_functions;

					bool directcomp_forced;
					
					Containerbase(asIScriptEngine* _engine, asIObjectType* _objtype) :
						aatc::container::shared::container_basicbase(_engine),
						container(_engine, this),
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

					void SetDirectcomp(bool ss) {
						if (directcomp_forced) { return; }
						handlemode_directcomp = ss;
					}

					//handle input is void* that points at handle which is a void*
					//handle output is void* that points at handle which is a void*
					void StoreHandle(void** target, void* ptr_to_handle) {
						*target = *(void**)ptr_to_handle;
						engine->AddRefScriptObject(*target, objtype_content);
					}
					void* StoreHandle2(void* ptr_to_handle) {
						void* result = *(void**)ptr_to_handle;
						engine->AddRefScriptObject(result, objtype_content);
						return result;
					}
					void ReleaseHandle(void* handle) {
						engine->ReleaseScriptObject(handle, objtype_content);
					}

					bool empty() {return container.empty();}
					aatc_type_sizetype Size() { return (aatc_type_sizetype)(container.size()); }

					void Clear() {
						safety_iteratorversion_Increment();

						iteratortype it = container.begin();
						iteratortype itend = container.end();
						if (handlemode) {
							for (; it != itend; it++) {
								ReleaseHandle(*it);
							}
						} else {
							for (; it != itend; it++) {
								engine->ReleaseScriptObject(*it, objtype_content);
							}
						}
						container.clear();
					}

					void EnumReferences(asIScriptEngine* engine) {
						if (astypeid_content & asTYPEID_MASK_OBJECT) {//dont do this for primitives
							iteratortype it = container.begin();
							iteratortype itend = container.end();
							for (; it != itend; it++) {
								engine->GCEnumCallback(*it);
							}
						}
					}
					void ReleaseAllReferences(asIScriptEngine* engine) {
						Clear();
					}











					class Iterator : public aatc::common::iterator_base {
					public:



						Containerbase* host;

						typename Containerbase::T_actual_container::iterator it;
						typename Containerbase::T_actual_container::iterator it_end;

						bool handlemode;

						Iterator() :
							it(),
							it_end()
						{}
						Iterator(void *ref, aatc_type_astypeid tid) :
							it(),
							it_end()
						{
							host = (Containerbase*)(*(void**)ref);
							Init();
						}
						Iterator(const Iterator& other) :
							iterator_base(other),

							host(other.host),
							it(other.it),
							it_end(other.it_end),

							handlemode(other.handlemode)
						{
							#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
								safety_iteratorversion = other.safety_iteratorversion;
							#endif
						}
						~Iterator() {}

						Iterator& operator=(const Iterator& other) {
							host = other.host;
							it = other.it;
							it_end = other.it_end;

							firstt = other.firstt;
							cont = other.cont;

							handlemode = other.handlemode;

							#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
								safety_iteratorversion = other.safety_iteratorversion;
							#endif

							return *this;
						}

						void Init() {
							#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
								safety_iteratorversion = host->safety_iteratorversion;
							#endif

							if (host->empty()) {
								cont = 0;

								it = host->container.end();
								it_end = host->container.end();
							} else {
								handlemode = host->handlemode;

								it = host->container.begin();
								it_end = host->container.end();
								cont = 1;
							}
						}

						//combine end check and continuation into one monster
						bool Next() {
							if (!IsValid()) {
								aatc::common::aatc_errorprint_iterator_container_modified();
								return 0;
							}

							if (firstt) {
								if (cont) {//all is well
									firstt = 0;
									return 1;
								} else {//cont set to 0 in constructor because container is empty
									return 0;
								}
							} else {
								it++;
								//if (it == host->end()){
								if (it == it_end) {
									return 0;
								} else {
									return 1;
								}
							}
						}



						template<typename T_dummy> const void* Current_get() {
							if (!IsValid()) {
								aatc::common::aatc_errorprint_iterator_container_modified();
								return nullptr;
							}

							if (handlemode) {
								return &(*it);//return pointer to handle
							} else {
								return *it;//return copy of pointer to object
							}
						}
						template<typename T_dummy> void Current_set(void* value) {
							if (!IsValid()) {
								aatc::common::aatc_errorprint_iterator_container_modified();
								return;
							}

							if (handlemode) {
								if (*it) {
									host->engine->ReleaseScriptObject(*it, host->objtype_content);
								}
								if (value) {
									*it = host->StoreHandle2(value);
								} else {
									*it = nullptr;
								}
							} else {
								host->engine->ReleaseScriptObject(*it, host->objtype_content);
								*it = host->engine->CreateScriptObjectCopy(value, host->objtype_content);
							}
						}

						template<typename T_dummy> void* Current() {
							if (!IsValid()) {
								aatc::common::aatc_errorprint_iterator_container_modified();
								return nullptr;
							}

							if (handlemode) {
								return &(*(it));//return pointer to handle
							} else {
								return *(it);//return copy of pointer to object
							}
						}

						template<typename T_dummy> const void* Current_const() {
							if (!IsValid()) {
								aatc::common::aatc_errorprint_iterator_container_modified();
								return nullptr;
							}

							if (handlemode) {
								return &(*(it));//return pointer to handle
							} else {
								return *(it);//return copy of pointer to object
							}
						}



						static void static_constructor_default(asIObjectType* objtype, void *memory) {
							new(memory)Iterator();
						}
						static void static_constructor_copy(asIObjectType* objtype, Iterator* other, void *memory) {
							new(memory)Iterator(*other);
						}
						static void static_constructor_parentcontainer(asIObjectType* objtype, void *ref, aatc_type_astypeid typeId, void *memory) {
							new(memory)Iterator(ref, typeId);
						}
						//static void static_constructor_copy(asIObjectType* objtype, const aatc_iterator& other, void *memory){
						//	new(memory)aatc_iterator(other);
						//}

						/*
						Using this in script should be faster than (it == container.end()) because container.end() creates an object
						*/
						bool IsEnd() {
							return it == it_end;
						}
						void SetToEnd() {
							firstt = 0;
							cont = 0;
							it = it_end;
						}
						bool IsValid() {
							#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
								return safety_iteratorversion == host->safety_iteratorversion;
							#else
								return 1;
							#endif
						}

						bool operator==(const Iterator& other) {
							return it == other.it;
						}

						template<class T_tag_need_const> static void Register_func_current(asIScriptEngine* engine, int& r, const char* n_iterator_T){}
						template<> static void Register_func_current<tag::iterator_access_is_mutable>(asIScriptEngine* engine, int& r, const char* n_iterator_T) {
							char textbuf[1000];
							sprintf_s(textbuf, 1000, "T& %s()", aatc_name_script_iterator_access_function);
							r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(Iterator, Current<int>), asCALL_THISCALL); assert(r >= 0);

							sprintf_s(textbuf, 1000, "T& get_%s()", aatc_name_script_iterator_access_property);
							r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(Iterator, Current_get<int>), asCALL_THISCALL); assert(r >= 0);
							sprintf_s(textbuf, 1000, "void set_%s(const T &in)", aatc_name_script_iterator_access_property);
							r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(Iterator, Current_set<int>), asCALL_THISCALL); assert(r >= 0);
						}
						template<> static void Register_func_current<tag::iterator_access_is_const>(asIScriptEngine* engine, int& r, const char* n_iterator_T) {
							char textbuf[1000];
							sprintf_s(textbuf, 1000, "const T& %s()", aatc_name_script_iterator_access_function);
							r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(Iterator, Current_const<int>), asCALL_THISCALL); assert(r >= 0);

							sprintf_s(textbuf, 1000, "const T& get_%s()", aatc_name_script_iterator_access_property);
							r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(Iterator, Current_get<int>), asCALL_THISCALL); assert(r >= 0);
						}



						static void Register(asIScriptEngine* engine, const char* n_iterator, const char* n_container_T) {
							int r = 0;
							char textbuf[1000];

							char n_iterator_T[1000];
							sprintf_s(n_iterator_T, 1000, "%s<T>", n_iterator);

							char n_iterator_class_T[1000];
							sprintf_s(n_iterator_class_T, 1000, "%s<class T>", n_iterator);

							r = engine->RegisterObjectType(n_iterator_class_T, sizeof(Iterator), asOBJ_VALUE | asOBJ_TEMPLATE | asGetTypeTraits<Iterator>()); assert(r >= 0);

							r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in)", asFunctionPtr(static_constructor_default), asCALL_CDECL_OBJLAST); assert(r >= 0);
							sprintf_s(textbuf, 1000, "void f(int&in,const %s &in)", n_iterator_T);
							r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(static_constructor_copy), asCALL_CDECL_OBJLAST); assert(r >= 0);
							r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in,?&in)", asFunctionPtr(static_constructor_parentcontainer), asCALL_CDECL_OBJLAST); assert(r >= 0);

							r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(aatc::common::reghelp::generic_destructor<Iterator>), asCALL_CDECL_OBJLAST); assert(r >= 0);

							Iterator::Register_func_current<T_container_tags::iterator_access>(engine, r, n_iterator_T);

							r = engine->RegisterObjectMethod(n_iterator_T, "bool next()", asMETHOD(Iterator, Next), asCALL_THISCALL); assert(r >= 0);
							r = engine->RegisterObjectMethod(n_iterator_T, "bool opPreInc()", asMETHOD(Iterator, Next), asCALL_THISCALL); assert(r >= 0);
							r = engine->RegisterObjectMethod(n_iterator_T, "bool opPostInc()", asMETHOD(Iterator, Next), asCALL_THISCALL); assert(r >= 0);

							sprintf_s(textbuf, 1000, "%s& opAssign(const %s &in)", n_iterator_T, n_iterator_T);
							r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(Iterator, operator=), asCALL_THISCALL); assert(r >= 0);

							sprintf_s(textbuf, 1000, "bool opEquals(const %s &in)", n_iterator_T);
							r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(Iterator, operator==), asCALL_THISCALL); assert(r >= 0);

							sprintf_s(textbuf, 1000, "bool %s()", aatc_name_script_iterator_method_is_end);
							r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(Iterator, IsEnd), asCALL_THISCALL); assert(r >= 0);

							sprintf_s(textbuf, 1000, "bool %s()", aatc_name_script_iterator_method_is_valid);
							r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(Iterator, IsValid), asCALL_THISCALL); assert(r >= 0);
						}
					};







				};

				namespace methods {

				};







			};//namespace templated
		};//namespace shared
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif