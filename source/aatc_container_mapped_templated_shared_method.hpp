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


#ifndef _includedh_aatc_container_mapped_templated_shared_method
#define _includedh_aatc_container_mapped_templated_shared_method



#include "aatc_container_mapped_templated_shared.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace mapped {
			namespace templated {
				namespace shared {



					namespace method {

						template<typename T_container> void swap(T_container* t, T_container& other) {
							t->container.swap(other.container);
							t->safety_iteratorversion_Increment();
							other.safety_iteratorversion_Increment();
						}


						template<typename T_container> void insert(T_container* t, void* newkey, void* newvalue) {
							#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
								if (t->need_errorcheck_missing_functions) {
									if (t->missing_functions & common::CONTAINER_OPERATION::INSERT) {
										common::errorprint::container::missingfunctions_operation_missing(t->objtype_container->GetName(), t->objtype_key->GetName(), "insert");
										return;
									}
								}
							#endif

							t->safety_iteratorversion_Increment();

							common::primunion findkey;

							t->BuildPrimunion(findkey, newkey, t->datahandlingid_key, t->primitiveid_key);

							T_container::T_iterator_native it = t->container.find(findkey);
							if (it == t->container.end()) {

								common::primunion_pair insertpair;

								switch (t->datahandlingid_key) {
								case common::DATAHANDLINGTYPE::STRING:
								{
									insertpair.first.ptr = t->engine->CreateScriptObjectCopy(newkey, t->objtype_key);
									break;
								}
								case common::DATAHANDLINGTYPE::HANDLE:
								{
									newkey = *(void**)newkey;
									t->StoreHandle(&(insertpair.first.ptr), newkey, t->objtype_key);
									break;
								}
								case common::DATAHANDLINGTYPE::OBJECT:
								{
									insertpair.first.ptr = t->engine->CreateScriptObjectCopy(newkey, t->objtype_key);
									break;
								}
								case common::DATAHANDLINGTYPE::PRIMITIVE:
								{
									switch (t->primitiveid_key) {
									case common::PRIMITIVE_TYPE::INT8: { insertpair.first.i8 = *((config::t::int8*)newkey); break; }
									case common::PRIMITIVE_TYPE::INT16: { insertpair.first.i16 = *((config::t::int16*)newkey); break; }
									case common::PRIMITIVE_TYPE::INT32: { insertpair.first.i32 = *((config::t::int32*)newkey); break; }
									case common::PRIMITIVE_TYPE::INT64: { insertpair.first.i64 = *((config::t::int64*)newkey); break; }
									case common::PRIMITIVE_TYPE::UINT8: { insertpair.first.ui8 = *((config::t::uint8*)newkey); break; }
									case common::PRIMITIVE_TYPE::UINT16: { insertpair.first.ui16 = *((config::t::uint16*)newkey); break; }
									case common::PRIMITIVE_TYPE::UINT32: { insertpair.first.ui32 = *((config::t::uint32*)newkey); break; }
									case common::PRIMITIVE_TYPE::UINT64: { insertpair.first.ui64 = *((config::t::uint64*)newkey); break; }
									case common::PRIMITIVE_TYPE::FLOAT32: { insertpair.first.f32 = *((config::t::float32*)newkey); break; }
									case common::PRIMITIVE_TYPE::FLOAT64: { insertpair.first.f64 = *((config::t::float64*)newkey); break; }
									};
									break;
								}
								};

								switch (t->datahandlingid_value) {
								case common::DATAHANDLINGTYPE::STRING:
								{
									insertpair.second.ptr = t->engine->CreateScriptObjectCopy(newvalue, t->objtype_value);
									break;
								}
								case common::DATAHANDLINGTYPE::HANDLE:
								{
									newvalue = *(void**)newvalue;
									t->StoreHandle(&(insertpair.second.ptr), newvalue, t->objtype_value);
									break;
								}
								case common::DATAHANDLINGTYPE::OBJECT:
								{
									insertpair.second.ptr = t->engine->CreateScriptObjectCopy(newvalue, t->objtype_value);
									break;
								}
								case common::DATAHANDLINGTYPE::PRIMITIVE:
								{
									switch (t->primitiveid_value) {
									case common::PRIMITIVE_TYPE::INT8: { insertpair.second.i8 = *((config::t::int8*)newvalue); break; }
									case common::PRIMITIVE_TYPE::INT16: { insertpair.second.i16 = *((config::t::int16*)newvalue); break; }
									case common::PRIMITIVE_TYPE::INT32: { insertpair.second.i32 = *((config::t::int32*)newvalue); break; }
									case common::PRIMITIVE_TYPE::INT64: { insertpair.second.i64 = *((config::t::int64*)newvalue); break; }
									case common::PRIMITIVE_TYPE::UINT8: { insertpair.second.ui8 = *((config::t::uint8*)newvalue); break; }
									case common::PRIMITIVE_TYPE::UINT16: { insertpair.second.ui16 = *((config::t::uint16*)newvalue); break; }
									case common::PRIMITIVE_TYPE::UINT32: { insertpair.second.ui32 = *((config::t::uint32*)newvalue); break; }
									case common::PRIMITIVE_TYPE::UINT64: { insertpair.second.ui64 = *((config::t::uint64*)newvalue); break; }
									case common::PRIMITIVE_TYPE::FLOAT32: { insertpair.second.f32 = *((config::t::float32*)newvalue); break; }
									case common::PRIMITIVE_TYPE::FLOAT64: { insertpair.second.f64 = *((config::t::float64*)newvalue); break; }
									};
									break;
								}
								};

								t->container.insert(insertpair);
							}
						}



						template<typename T_container> void erase(T_container* t, void* key) {
							#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
								if (t->need_errorcheck_missing_functions) {
									if (t->missing_functions & common::CONTAINER_OPERATION::ERASE_VALUE) {
										common::errorprint::container::missingfunctions_operation_missing(t->objtype_container->GetName(), t->objtype_key->GetName(), "erase");
										return;
									}
								}
							#endif

							t->safety_iteratorversion_Increment();

							common::primunion findkey;

							t->BuildPrimunion(findkey, key, t->datahandlingid_key, t->primitiveid_key);

							T_container::T_iterator_native it = t->container.find(findkey);

							if (it != t->container.end()) {
								common::primunion old_key;
								common::primunion old_value;

								if (t->datahandlingid_key != common::DATAHANDLINGTYPE::PRIMITIVE) { old_key.ptr = (*it).first.ptr; }
								if (t->datahandlingid_value != common::DATAHANDLINGTYPE::PRIMITIVE) { old_value.ptr = (*it).second.ptr; }

								t->container.erase(it);

								switch (t->datahandlingid_key) {
								case common::DATAHANDLINGTYPE::PRIMITIVE: { break; }
								case common::DATAHANDLINGTYPE::STRING:
								{
									//delete ((config::t::string*)old_key.ptr);
									t->engine->ReleaseScriptObject(old_key.ptr, t->objtype_key);
									break;
								}
								default:
								{
									t->engine->ReleaseScriptObject(old_key.ptr, t->objtype_key);
									break;
								}
								};
								switch (t->datahandlingid_value) {
								case common::DATAHANDLINGTYPE::PRIMITIVE: { break; }
								case common::DATAHANDLINGTYPE::STRING:
								{
									//delete ((config::t::string*)old_value.ptr);
									t->engine->ReleaseScriptObject(old_value.ptr, t->objtype_value);
									break;
								}
								default:
								{
									t->engine->ReleaseScriptObject(old_value.ptr, t->objtype_value);
									break;
								}
								};
							}
						}


						template<typename T_container> void* find_value(T_container* t, void* key, bool& success) {
							#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
								if (t->need_errorcheck_missing_functions) {
									if (t->missing_functions & common::CONTAINER_OPERATION::FIND) {
										common::errorprint::container::missingfunctions_operation_missing(t->objtype_container->GetName(), t->objtype_key->GetName(), "find");
										return T_container::DefaultPrimunion(t->datahandlingid_value, t->primitiveid_value);
									}
								}
							#endif

							common::primunion findkey;
							t->BuildPrimunion(findkey, key, t->datahandlingid_key, t->primitiveid_key);

							T_container::T_iterator_native_const it = t->container.find(findkey);
							if (it == t->container.end()) {
								success = 0;

								return T_container::DefaultPrimunion(t->datahandlingid_value, t->primitiveid_value);
							} else {
								success = 1;

								const common::primunion& found_value_const = it->second;
								common::primunion& found_value = const_cast<common::primunion&>(found_value_const);

								return T_container::Primunion_to_Scriptany(found_value, t->datahandlingid_value, t->primitiveid_value);
							}
						}

						template<typename T_container> void* find_value(T_container* t, void* key) {
							bool success;
							return find_value(t, key, success);
						}

						template<typename T_container> bool contains(T_container* t, void* key) {
							bool find_success = 0;
							find_value(t, key, find_success);
							return find_success;
						}






						template<typename T_container> typename T_container::Iterator find_iterator(T_container* t, void* key) {
							#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
								if (t->need_errorcheck_missing_functions) {
									if (t->missing_functions & common::CONTAINER_OPERATION::FIND) {
										common::errorprint::container::missingfunctions_operation_missing(t->objtype_container->GetName(), t->objtype_key->GetName(), "find");
										return t->end();
									}
								}
							#endif

							common::primunion findkey;
							t->BuildPrimunion(findkey, key, t->datahandlingid_key, t->primitiveid_key);

							T_container::T_iterator_native it = t->container.find(findkey);


							T_container::Iterator result(t);
							result.it = it;

							if (it == t->container.end()) {
								result.SetToEnd();
							}

							return result;
						}

						template<typename T_container> bool erase_iterator(T_container* t, typename const T_container::Iterator& aatc_it) {
							#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
								if (t->safety_iteratorversion != aatc_it.safety_iteratorversion) {
									common::errorprint::container::iterator_invalid();
									return 0;
								}
							#endif

							T_container::T_iterator_native it = aatc_it.it;

							if (it == t->container.end()) {
								return 0;
							} else {
								t->safety_iteratorversion_Increment();

								common::primunion old_key;
								common::primunion old_value;

								if (t->datahandlingid_key != common::DATAHANDLINGTYPE::PRIMITIVE) { old_key.ptr = (*it).first.ptr; }
								if (t->datahandlingid_value != common::DATAHANDLINGTYPE::PRIMITIVE) { old_value.ptr = (*it).second.ptr; }

								t->container.erase(it);

								switch (t->datahandlingid_key) {
								case common::DATAHANDLINGTYPE::PRIMITIVE: { break; }
								case common::DATAHANDLINGTYPE::STRING:
								{
									t->engine->ReleaseScriptObject(old_key.ptr, t->objtype_key);
									break;
								}
								default:
								{
									t->engine->ReleaseScriptObject(old_key.ptr, t->objtype_key);
									break;
								}
								};
								switch (t->datahandlingid_value) {
								case common::DATAHANDLINGTYPE::PRIMITIVE: { break; }
								case common::DATAHANDLINGTYPE::STRING:
								{
									t->engine->ReleaseScriptObject(old_value.ptr, t->objtype_value);
									break;
								}
								default:
								{
									t->engine->ReleaseScriptObject(old_value.ptr, t->objtype_value);
									break;
								}
								};

								return 1;
							}
						}

						template<typename T_container> config::t::sizetype erase_iterator_range(T_container* t, typename const T_container::Iterator& aatc_it_range_begin, typename const T_container::Iterator& aatc_it_range_end) {
							#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
								if ((t->safety_iteratorversion != aatc_it_range_begin.safety_iteratorversion) || (t->safety_iteratorversion != aatc_it_range_end.safety_iteratorversion)) {
									common::errorprint::container::iterator_invalid();
									return 0;
								}
							#endif


							T_container::T_iterator_native it_range_begin = aatc_it_range_begin.it;
							T_container::T_iterator_native it_range_end = aatc_it_range_end.it;

							if (it_range_begin == it_range_end) {
								return 0;
							} else {
								t->safety_iteratorversion_Increment();

								config::t::sizetype delcount = (config::t::sizetype)std::distance(it_range_begin, it_range_end);

								std::vector<std::pair<common::primunion, common::primunion>> old_items;

								int nonprimitives = (t->datahandlingid_key != common::DATAHANDLINGTYPE::PRIMITIVE) + (t->datahandlingid_value != common::DATAHANDLINGTYPE::PRIMITIVE);

								if (nonprimitives) {
									old_items.reserve(delcount);

									if (nonprimitives == 2) {
										for (auto it = it_range_begin; it != it_range_end; it++) {
											std::pair<common::primunion, common::primunion> pp;
											pp.first.ptr = (*it).first.ptr;
											pp.second.ptr = (*it).second.ptr;
											old_items.push_back(pp);
										}
									} else {
										if (t->datahandlingid_key != common::DATAHANDLINGTYPE::PRIMITIVE) {
											for (auto it = it_range_begin; it != it_range_end; it++) {
												std::pair<common::primunion, common::primunion> pp;
												pp.first.ptr = (*it).first.ptr;
												old_items.push_back(pp);
											}
										} else {
											for (auto it = it_range_begin; it != it_range_end; it++) {
												std::pair<common::primunion, common::primunion> pp;
												pp.second.ptr = (*it).second.ptr;
												old_items.push_back(pp);
											}
										}
									}
								}

								t->container.erase(it_range_begin, it_range_end);

								if (nonprimitives) {
									if (nonprimitives == 2) {
										for (auto it = old_items.begin(); it != old_items.end(); it++) {
											t->engine->ReleaseScriptObject((*it).first.ptr, t->objtype_key);
											t->engine->ReleaseScriptObject((*it).second.ptr, t->objtype_value);
										}
									} else {
										if (t->datahandlingid_key != common::DATAHANDLINGTYPE::PRIMITIVE) {
											for (auto it = old_items.begin(); it != old_items.end(); it++) {
												t->engine->ReleaseScriptObject((*it).first.ptr, t->objtype_key);
											}
										} else {
											for (auto it = old_items.begin(); it != old_items.end(); it++) {
												t->engine->ReleaseScriptObject((*it).second.ptr, t->objtype_value);
											}
										}
									}
								}

								return delcount;
							}
						}



						namespace cpp_interface {
							template<typename T_container> void* operator_index(T_container* t, void* key) {
								bool find_success = 0;
								void* find_result = find_value(t, key, find_success);

								if (success) {
									return find_result;
								} else {
									common::primunion_pair insertpair;

									t->Scriptany_to_Primunion(key, insertpair.first, t->datahandlingid_key, t->primitiveid_key, t->objtype_key);
									t->DefaultConstructPrimunion(insertpair.second, t->datahandlingid_value, t->primitiveid_value, t->objtype_value);

									std::pair<T_container::T_iterator_native,bool> insertpair = t->container.insert(insertpair);

									return T_container::Primunion_to_Scriptany(insertpair.first->second, t->datahandlingid_value, t->primitiveid_value);
								}
							}
						};



					};//namespace method



					namespace register_method {

						template<typename T_container> static void swap(common::RegistrationState& rs) {
							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "%s& %s(%s@)", rs.n_container_T, config::scriptname::method::container::swap, rs.n_container_T);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::swap<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
						}

						template<typename T_container> static void insert(common::RegistrationState& rs) {
							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "void %s(const T_key&in,const T_value&in)", config::scriptname::method::container::insert);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::insert<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
						}

						template<typename T_container> static void erase(common::RegistrationState& rs) {
							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "void %s(const T_key&in)", config::scriptname::method::container::erase);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::erase<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
						}

						template<typename T_container> static void find(common::RegistrationState& rs) {
							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "T_value& %s(const T_key &in)", config::scriptname::method::container::find_value);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTIONPR(method::find_value<T_container>,(T_container*, void*),void*), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);

							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "T_value& %s(const T_key &in,bool &out)", config::scriptname::method::container::find_value);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTIONPR(method::find_value<T_container>, (T_container*, void*, bool&), void*), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);

							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "bool %s(const T_key&in)", config::scriptname::method::container::contains);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::contains<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
						}

						template<typename T_container> static void find_iterator(common::RegistrationState& rs) {
							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "%s %s(const T_key &in)", rs.n_iterator_T, config::scriptname::method::container::find_iterator);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::find_iterator<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
						}

						template<typename T_container> static void erase_iterator(common::RegistrationState& rs) {
							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "bool %s(const %s &in)", config::scriptname::method::container::erase_iterator, rs.n_iterator_T);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::erase_iterator<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
						}
						template<typename T_container> static void erase_iterator_range(common::RegistrationState& rs) {
							sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "%s %s(const %s &in,const %s &in)", config::scriptname::t::size, config::scriptname::method::container::erase_iterator, rs.n_iterator_T, rs.n_iterator_T);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::erase_iterator_range<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
						}

						template<typename T_container> static void operator_index(common::RegistrationState& rs) {
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, "const T_value& get_opIndex(const T_key &in) const", asFUNCTIONPR(method::find_value<T_container>, (T_container*, void*), void*), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
							rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, "void set_opIndex(const T_key&in,const T_value&in)", asFUNCTION(method::insert<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
						}



					};//namespace register_method



				};//namespace shared
			};//namespace templated
		};//namespace mapped
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif