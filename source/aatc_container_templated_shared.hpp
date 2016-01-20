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


#ifndef _includedh_aatc_container_templated_shared
#define _includedh_aatc_container_templated_shared



#include "aatc_common.hpp"
#include "aatc_container_shared.hpp"
#include "aatc_container_listing.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace templated {
			namespace shared {



				//doxygen skip
				#ifndef DOXYGEN_SHOULD_SKIP_THIS

				//we need a wrapper around the base container so that we can
				//feed the constructor a fancy functor to handle internal functions that require script functions to be called
				namespace base_container_wrapper {
					template<class T_container> class Basic : public T_container {
					public:
						Basic(asIScriptEngine* engine, aatc::container::shared::containerfunctor::Settings* settings) {}
					};

					template<class T_container, class T_param1> class Singleparam : public T_container {
					public:
						Singleparam(asIScriptEngine* _engine, aatc::container::shared::containerfunctor::Settings* settings) :
							T_container(T_param1(_engine, settings))
						{}
					};

					template<class T_container, class T_param1, class T_param2, std::size_t default_bucket_count> class Hashed : public T_container {
					public:
						Hashed(asIScriptEngine* _engine, aatc::container::shared::containerfunctor::Settings* settings) :
							T_container(default_bucket_count, T_param1(_engine, settings), T_param2(_engine, settings))
						{}
					};
				};

				#endif
				//doxygen skip



				template<typename T_container> T_container* Factory(asIObjectType* objtype) {
					return new T_container(objtype);
				}
				template<typename T_container> T_container* Factory_copy(asIObjectType* objtype, const T_container& other) {
					return new T_container(other);
				}



				/*!\brief Internal template monster

				*/
				template<
					typename T_container,
					int T_CONTAINERTYPEID,
					typename T_container_tags = aatc::container::shared::tagbase,
					typename bcw = base_container_wrapper::Basic<T_container>>
				class Containerbase :
					public container::shared::container_basicbase,
					public common::aatc_refcounted_GC,
					public container::shared::containerfunctor::Settings
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

					virtual ~Containerbase() {
						clear();
					}

					void operator=(const Containerbase& other) {
						safety_iteratorversion_Increment();

						clear();//will delete script objects or decrement handles

						container.operator=(other.container);

						if (handlemode) {//increment refs for newly created handles
							for (auto it = container.begin(); it != container.end(); it++) {
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

							iteratortype it = container.begin();
							iteratortype it_other = const_cast<Containerbase&>(other).container.begin();
							iteratortype it_end = container.end();

							for (; it != it_end;) {
								void*& iii = const_cast<void*&>(*it);
								void*& iii_other = const_cast<void*&>(*it_other);

								iii = engine->CreateScriptObjectCopy(iii_other, objtype_content);

								it++;
								it_other++;
							}
						}
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
					aatc_type_sizetype size() { return (aatc_type_sizetype)(container.size()); }

					void clear() {
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

					aatc_type_sizetype count(void* value) {
						if (handlemode_directcomp) {
							return (aatc_type_sizetype)(std::count(container.begin(), container.end(), *(void**)value));
						} else {
							#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
								if (missing_functions & aatc::common::aatc_CONTAINER_OPERATION::COUNT) {
									aatc::common::aatc_errorprint_container_missingfunctions_operation_missing(objtype_container->GetName(), objtype_content->GetName(), "count");
									return NULL;
								}
							#endif


							if (handlemode) { value = *(void**)value; }

							asIScriptContext* cc = els->contextcache_Get();

							aatc_type_sizetype count = 0;
							iteratortype it = container.begin();
							iteratortype itend = container.end();

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
						clear();
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

						template<class T_tag_need_const> static void Register_func_current(common::RegistrationState& rs) {}
						template<> static void Register_func_current<container::shared::tag::iterator_access_is_mutable>(common::RegistrationState& rs) {
							sprintf_s(rs.textbuf, 1000, "T& %s()", aatc_name_script_iterator_access_function);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, rs.textbuf, asMETHOD(Iterator, Current<int>), asCALL_THISCALL); assert(rs.error >= 0);

							sprintf_s(rs.textbuf, 1000, "T& get_%s()", aatc_name_script_iterator_access_property);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, rs.textbuf, asMETHOD(Iterator, Current_get<int>), asCALL_THISCALL); assert(rs.error >= 0);
							sprintf_s(rs.textbuf, 1000, "void set_%s(const T &in)", aatc_name_script_iterator_access_property);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, rs.textbuf, asMETHOD(Iterator, Current_set<int>), asCALL_THISCALL); assert(rs.error >= 0);
						}
						template<> static void Register_func_current<container::shared::tag::iterator_access_is_const>(common::RegistrationState& rs) {
							sprintf_s(rs.textbuf, 1000, "const T& %s()", aatc_name_script_iterator_access_function);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, rs.textbuf, asMETHOD(Iterator, Current_const<int>), asCALL_THISCALL); assert(rs.error >= 0);

							sprintf_s(rs.textbuf, 1000, "const T& get_%s()", aatc_name_script_iterator_access_property);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, rs.textbuf, asMETHOD(Iterator, Current_get<int>), asCALL_THISCALL); assert(rs.error >= 0);
						}



						static void Register(common::RegistrationState& rs) {
							rs.error = rs.engine->RegisterObjectType(rs.n_iterator_class_T, sizeof(Iterator), asOBJ_VALUE | asOBJ_TEMPLATE | asGetTypeTraits<Iterator>()); assert(rs.error >= 0);

							rs.error = rs.engine->RegisterObjectBehaviour(rs.n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in)", asFunctionPtr(static_constructor_default), asCALL_CDECL_OBJLAST); assert(rs.error >= 0);
							sprintf_s(rs.textbuf, 1000, "void f(int&in,const %s &in)", rs.n_iterator_T);
							rs.error = rs.engine->RegisterObjectBehaviour(rs.n_iterator_T, asBEHAVE_CONSTRUCT, rs.textbuf, asFunctionPtr(static_constructor_copy), asCALL_CDECL_OBJLAST); assert(rs.error >= 0);
							rs.error = rs.engine->RegisterObjectBehaviour(rs.n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in,?&in)", asFunctionPtr(static_constructor_parentcontainer), asCALL_CDECL_OBJLAST); assert(rs.error >= 0);

							rs.error = rs.engine->RegisterObjectBehaviour(rs.n_iterator_T, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(aatc::common::reghelp::generic_destructor<Iterator>), asCALL_CDECL_OBJLAST); assert(rs.error >= 0);

							Iterator::Register_func_current<T_container_tags::iterator_access>(rs);

							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, "bool next()", asMETHOD(Iterator, Next), asCALL_THISCALL); assert(rs.error >= 0);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, "bool opPreInc()", asMETHOD(Iterator, Next), asCALL_THISCALL); assert(rs.error >= 0);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, "bool opPostInc()", asMETHOD(Iterator, Next), asCALL_THISCALL); assert(rs.error >= 0);

							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "%s& opAssign(const %s &in)", rs.n_iterator_T, rs.n_iterator_T);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, rs.textbuf, asMETHOD(Iterator, operator=), asCALL_THISCALL); assert(rs.error >= 0);

							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "bool opEquals(const %s &in)", rs.n_iterator_T);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, rs.textbuf, asMETHOD(Iterator, operator==), asCALL_THISCALL); assert(rs.error >= 0);

							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "bool %s()", aatc_name_script_iterator_method_is_end);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, rs.textbuf, asMETHOD(Iterator, IsEnd), asCALL_THISCALL); assert(rs.error >= 0);

							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "bool %s()", aatc_name_script_iterator_method_is_valid);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_iterator_T, rs.textbuf, asMETHOD(Iterator, IsValid), asCALL_THISCALL); assert(rs.error >= 0);
						}
					};



				};





				template<typename T_container> void register_containerbase(aatc::common::RegistrationState& rs, const char* n_container) {
					sprintf_s(rs.n_container, aatc::common::RegistrationState::bufsize, "%s", n_container);
					sprintf_s(rs.n_container_T, aatc::common::RegistrationState::bufsize, "%s<T>", n_container);
					sprintf_s(rs.n_container_class_T, aatc::common::RegistrationState::bufsize, "%s<class T>", n_container);
					sprintf_s(rs.n_iterator, aatc::common::RegistrationState::bufsize, "%s%s", n_container, aatc_name_script_iterator);
					sprintf_s(rs.n_iterator_T, aatc::common::RegistrationState::bufsize, "%s<T>", rs.n_iterator);
					sprintf_s(rs.n_iterator_class_T, aatc::common::RegistrationState::bufsize, "%s<class T>", rs.n_iterator);


					rs.error = rs.engine->RegisterObjectType(rs.n_container_class_T, 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE); assert(rs.error >= 0);

					sprintf_s(rs.textbuf, aatc::common::RegistrationState::bufsize, "%s@ f(int&in)", rs.n_container_T);
					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_FACTORY, rs.textbuf, asFUNCTIONPR(aatc::container::templated::shared::Factory<T_container>, (asIObjectType*), T_container*), asCALL_CDECL); assert(rs.error >= 0);
					sprintf_s(rs.textbuf, aatc::common::RegistrationState::bufsize, "%s@ f(int&in,const %s &in)", rs.n_container_T, rs.n_container_T);
					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_FACTORY, rs.textbuf, asFUNCTIONPR(aatc::container::templated::shared::Factory_copy<T_container>, (asIObjectType*, const T_container&), T_container*), asCALL_CDECL); assert(rs.error >= 0);
					sprintf_s(rs.textbuf, aatc::common::RegistrationState::bufsize, "%s& opAssign(const %s &in)", rs.n_container_T, rs.n_container_T);
					rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asMETHOD(T_container, operator=), asCALL_THISCALL); assert(rs.error >= 0);

					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_ADDREF, "void f()", asMETHOD(T_container, refcount_Add), asCALL_THISCALL); assert(rs.error >= 0);
					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_RELEASE, "void f()", asMETHOD(T_container, refcount_Release), asCALL_THISCALL); assert(rs.error >= 0);
					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_SETGCFLAG, "void f()", asMETHODPR(T_container, SetGCFlag, (), void), asCALL_THISCALL); assert(rs.error >= 0);
					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_GETGCFLAG, "bool f()", asMETHODPR(T_container, GetGCFlag, (), bool), asCALL_THISCALL); assert(rs.error >= 0);
					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_GETREFCOUNT, "int f()", asMETHODPR(T_container, GetRefCount, (), int), asCALL_THISCALL); assert(rs.error >= 0);
					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_ENUMREFS, "void f(int&in)", asMETHODPR(T_container, EnumReferences, (asIScriptEngine*), void), asCALL_THISCALL); assert(rs.error >= 0);
					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHODPR(T_container, ReleaseAllReferences, (asIScriptEngine*), void), asCALL_THISCALL); assert(rs.error >= 0);


					rs.error = rs.engine->RegisterObjectBehaviour(rs.n_container_T, asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(aatc::common::aatc_templatecallback_1tp), asCALL_CDECL); assert(rs.error >= 0);

					sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "void %s(bool)", aatc_name_script_container_method_set_directcomp);
					rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asMETHOD(T_container, SetDirectcomp), asCALL_THISCALL); assert(rs.error >= 0);

					sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "%s %s(const T &in)", aatc_name_script_sizetype, aatc_name_script_container_method_count);
					rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asMETHOD(T_container, count), asCALL_THISCALL); assert(rs.error >= 0);

					sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "void %s()", aatc_name_script_container_method_clear);
					rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asMETHOD(T_container, clear), asCALL_THISCALL); assert(rs.error >= 0);
					sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "bool %s()", aatc_name_script_container_method_empty);
					rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asMETHOD(T_container, empty), asCALL_THISCALL); assert(rs.error >= 0);
					sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "%s %s()", aatc_name_script_sizetype, aatc_name_script_container_method_size);
					rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asMETHOD(T_container, size), asCALL_THISCALL); assert(rs.error >= 0);

				};







			};//namespace shared
		};//namespace templated
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif