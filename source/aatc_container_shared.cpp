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



#include "aatc_container_shared.hpp"
#include "aatc_hash.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace shared {



			container_basicbase::container_basicbase(asIScriptEngine* engine):
				engine(engine)
			{
				#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
					safety_iteratorversion = 0;
				#endif
			}
			container_basicbase::~container_basicbase() {}

			void container_basicbase::safety_iteratorversion_Increment() {
				#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
					safety_iteratorversion++;
				#endif
			}


			namespace containerfunctor {
				Base::Base(asIScriptEngine* _engine, Settings* settings) :
					engine(_engine),
					host_settings(settings),
					need_init(1)
				{
					els = (aatc::common::engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);
				}

				Comp::Comp(asIScriptEngine* _engine, Settings* settings) :
					Base(_engine, settings)
				{}
				bool Comp::operator()(const void* lhs, const void* rhs)const {
					if (need_init) {
						(const_cast<Comp*>(this))->need_init = 0;
						(const_cast<Comp*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
						(const_cast<Comp*>(this))->func_cmp = host_settings->func_cmp;
					}
					if (handlemode_directcomp) {
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

				Equals::Equals(asIScriptEngine* _engine, Settings* settings) :
					Base(_engine, settings)
				{}
				bool Equals::operator()(const void* lhs, const void* rhs)const {
					if (need_init) {
						(const_cast<Equals*>(this))->need_init = 0;
						(const_cast<Equals*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
						(const_cast<Equals*>(this))->func_equals = host_settings->func_equals;
					}
					if (handlemode_directcomp) {
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
				bool Equals::findif_version::operator()(const void* rhs) const {
					return (*f)(target, rhs);
				}

				Hash::Hash(asIScriptEngine* _engine, Settings* settings) :
					Base(_engine, settings)
				{}
				aatc_hash_type Hash::operator()(const void* ptr) const {
					if (need_init) {
						//need_init = 0;
						//func_hash = host_settings->func_hash;
						//handlemode_directcomp = host_settings->handlemode_directcomp;
						(const_cast<Hash*>(this))->need_init = 0;
						(const_cast<Hash*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
						(const_cast<Hash*>(this))->func_hash = host_settings->func_hash;
					}
					if (handlemode_directcomp) {
						return (aatc_hash_type)ptr;
						//return reinterpret_cast<std::size_t>(ptr);
					}

					aatc_hash_type result;

					asIScriptContext* cc = els->contextcache_Get();
					cc->Prepare(func_hash);
					cc->SetObject(const_cast<void*>(ptr));
					if (cc->Execute() == asEXECUTION_EXCEPTION) {
						result = 0;
					} else {
						result = cc->GetReturnQWord();
					}
					els->contextcache_Return(cc);

					return result;
				}


			};
			namespace containerfunctor_map {
				Base::Base(asIScriptEngine* _engine, Settings* settings) :
					engine(_engine),
					host_settings(settings),
					need_init(1)
				{
					els = (aatc::common::engine_level_storage*)engine->GetUserData(aatc_engine_userdata_id);
				}

				Comp::Comp(asIScriptEngine* _engine, Settings* settings) :
					Base(_engine,settings)
				{}
				bool Comp::operator()(const aatc::common::primunion& lhs, const aatc::common::primunion& rhs)const {
					if (need_init) {
						(const_cast<Comp*>(this))->need_init = 0;
						(const_cast<Comp*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
						(const_cast<Comp*>(this))->datahandlingid_key = host_settings->datahandlingid_key;
						(const_cast<Comp*>(this))->primitiveid_key = host_settings->primitiveid_key;
						(const_cast<Comp*>(this))->func_cmp = host_settings->func_cmp;
					}
					if (datahandlingid_key == aatc::common::DATAHANDLINGTYPE::PRIMITIVE) {
						switch (primitiveid_key) {
						case aatc::common::PRIMITIVE_TYPE::INT8: { return lhs.i8 < rhs.i8; }
						case aatc::common::PRIMITIVE_TYPE::INT16: { return lhs.i16 < rhs.i16; }
						case aatc::common::PRIMITIVE_TYPE::INT32: { return lhs.i32 < rhs.i32; }
						case aatc::common::PRIMITIVE_TYPE::INT64: { return lhs.i64 < rhs.i64; }
						case aatc::common::PRIMITIVE_TYPE::UINT8: { return lhs.ui8 < rhs.ui8; }
						case aatc::common::PRIMITIVE_TYPE::UINT16: { return lhs.ui16 < rhs.ui16; }
						case aatc::common::PRIMITIVE_TYPE::UINT32: { return lhs.ui32 < rhs.ui32; }
						case aatc::common::PRIMITIVE_TYPE::UINT64: { return lhs.ui64 < rhs.ui64; }
						case aatc::common::PRIMITIVE_TYPE::FLOAT32: { return lhs.f32 < rhs.f32; }
						case aatc::common::PRIMITIVE_TYPE::FLOAT64: { return lhs.f64 < rhs.f64; }
						};
					}
					if (datahandlingid_key == aatc::common::DATAHANDLINGTYPE::STRING) {
						return (*((aatc_type_string*)lhs.ptr)) < (*((aatc_type_string*)rhs.ptr));
					}
					if (handlemode_directcomp) {
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


				Equals::Equals(asIScriptEngine* _engine, Settings* settings) :
					Base(_engine, settings)
				{}
				bool Equals::operator()(const aatc::common::primunion& lhs, const aatc::common::primunion& rhs)const {
					if (need_init) {
						(const_cast<Equals*>(this))->need_init = 0;
						(const_cast<Equals*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
						(const_cast<Equals*>(this))->datahandlingid_key = host_settings->datahandlingid_key;
						(const_cast<Equals*>(this))->primitiveid_key = host_settings->primitiveid_key;
						(const_cast<Equals*>(this))->func_equals = host_settings->func_equals;
					}
					if (datahandlingid_key == aatc::common::DATAHANDLINGTYPE::PRIMITIVE) {
						switch (primitiveid_key) {
						case aatc::common::PRIMITIVE_TYPE::INT8: { return lhs.i8 == rhs.i8; }
						case aatc::common::PRIMITIVE_TYPE::INT16: { return lhs.i16 == rhs.i16; }
						case aatc::common::PRIMITIVE_TYPE::INT32: { return lhs.i32 == rhs.i32; }
						case aatc::common::PRIMITIVE_TYPE::INT64: { return lhs.i64 == rhs.i64; }
						case aatc::common::PRIMITIVE_TYPE::UINT8: { return lhs.ui8 == rhs.ui8; }
						case aatc::common::PRIMITIVE_TYPE::UINT16: { return lhs.ui16 == rhs.ui16; }
						case aatc::common::PRIMITIVE_TYPE::UINT32: { return lhs.ui32 == rhs.ui32; }
						case aatc::common::PRIMITIVE_TYPE::UINT64: { return lhs.ui64 == rhs.ui64; }
						case aatc::common::PRIMITIVE_TYPE::FLOAT32: { return lhs.f32 == rhs.f32; }
						case aatc::common::PRIMITIVE_TYPE::FLOAT64: { return lhs.f64 == rhs.f64; }
						};
					}
					if (datahandlingid_key == aatc::common::DATAHANDLINGTYPE::STRING) {
						return (*((aatc_type_string*)lhs.ptr)) == (*((aatc_type_string*)rhs.ptr));
					}
					if (handlemode_directcomp) {
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

				Hash::Hash(asIScriptEngine* _engine, Settings* settings) :
					Base(_engine, settings)
				{}
				aatc_hash_type Hash::operator()(const aatc::common::primunion& pu) const {
					if (need_init) {
						//need_init = 0;
						//func_hash = host_settings->func_hash;
						//handlemode_directcomp = host_settings->handlemode_directcomp;
						(const_cast<Hash*>(this))->need_init = 0;
						(const_cast<Hash*>(this))->handlemode_directcomp = host_settings->handlemode_directcomp;
						(const_cast<Hash*>(this))->datahandlingid_key = host_settings->datahandlingid_key;
						(const_cast<Hash*>(this))->primitiveid_key = host_settings->primitiveid_key;
						(const_cast<Hash*>(this))->func_hash = host_settings->func_hash;
					}
					if (datahandlingid_key == aatc::common::DATAHANDLINGTYPE::PRIMITIVE) {
						switch (primitiveid_key) {
						case aatc::common::PRIMITIVE_TYPE::INT8: { return (aatc_hash_type)pu.i8; }
						case aatc::common::PRIMITIVE_TYPE::INT16: { return (aatc_hash_type)pu.i16; }
						case aatc::common::PRIMITIVE_TYPE::INT32: { return (aatc_hash_type)pu.i32; }
						case aatc::common::PRIMITIVE_TYPE::INT64: { return (aatc_hash_type)pu.i64; }
						case aatc::common::PRIMITIVE_TYPE::UINT8: { return (aatc_hash_type)pu.ui8; }
						case aatc::common::PRIMITIVE_TYPE::UINT16: { return (aatc_hash_type)pu.ui16; }
						case aatc::common::PRIMITIVE_TYPE::UINT32: { return (aatc_hash_type)pu.ui32; }
						case aatc::common::PRIMITIVE_TYPE::UINT64: { return (aatc_hash_type)pu.ui64; }
						case aatc::common::PRIMITIVE_TYPE::FLOAT32: { return (aatc_hash_type)pu.f32; }
						case aatc::common::PRIMITIVE_TYPE::FLOAT64: { return (aatc_hash_type)pu.f64; }
						};
					}
					if (datahandlingid_key == aatc::common::DATAHANDLINGTYPE::STRING) {
						return aatc::hash::hashfunctor<aatc_type_string>()(*((aatc_type_string*)pu.ptr));
					}
					if (handlemode_directcomp) {
						return (aatc_hash_type)pu.ptr;
						//return reinterpret_cast<std::size_t>(ptr);
					}

					aatc_hash_type result;

					asIScriptContext* cc = els->contextcache_Get();
					cc->Prepare(func_hash);
					cc->SetObject(const_cast<void*>(pu.ptr));
					if (cc->Execute() == asEXECUTION_EXCEPTION) {
						result = 0;
					} else {
						result = cc->GetReturnQWord();
					}
					els->contextcache_Return(cc);

					return result;
				}
				//MAP FUNCTORS
				//MAP FUNCTORS
				//MAP FUNCTORS
			};//namespace detail







		};//namespace shared
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE