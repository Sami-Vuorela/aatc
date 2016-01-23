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


#ifndef _includedh_aatc_container_shared
#define _includedh_aatc_container_shared



#include "aatc_common.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace shared {



			namespace tag {
				struct iterator_access_is_const {};
				struct iterator_access_is_mutable {};
			};

			class tagbase {
			public:
				typedef tag::iterator_access_is_mutable iterator_access;
			};



			class container_basicbase {
			public:
				asIScriptEngine* engine;

				#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
					int8_t safety_iteratorversion;
				#endif

				container_basicbase(asIScriptEngine* engine);
				virtual ~container_basicbase();

				void safety_iteratorversion_Increment();
			};



			namespace containerfunctor {
				class Settings {
				public:
					bool handlemode;
					bool handlemode_directcomp;
					asIScriptFunction* func_cmp;
					asIScriptFunction* func_equals;
					asIScriptFunction* func_hash;
				};
				class Base {
				public:
					asIScriptEngine* engine;
					enginestorage::engine_level_storage* els;
					Settings* host_settings;

					bool need_init;

					Base(asIScriptEngine* engine, Settings* settings);
				};

				class Comp : public Settings, public Base {
				public:
					Comp(asIScriptEngine* engine, Settings* settings);
					bool operator()(const void* lhs, const void* rhs) const;
				};
				class Equals : public Settings, public Base {
				public:
					Equals(asIScriptEngine* engine, Settings* settings);
					bool operator()(const void* lhs, const void* rhs) const;

					class findif_version {
					public:
						void* target;
						Equals* f;

						bool operator()(const void* rhs) const;
					};
				};
				class Hash : public Settings, public Base {
				public:
					Hash(asIScriptEngine* engine, Settings* settings);
					config::t::hash operator()(const void* ptr) const;
				};
			};//namespace containerfunctor
			namespace containerfunctor_map {
				class Settings {
				public:
					bool handlemode_directcomp;
					asIScriptFunction* func_cmp;
					asIScriptFunction* func_equals;
					asIScriptFunction* func_hash;

					aatc::common::DATAHANDLINGTYPE datahandlingid_key;
					aatc::common::PRIMITIVE_TYPE primitiveid_key;
				};
				class Base {
				public:
					asIScriptEngine* engine;
					enginestorage::engine_level_storage* els;
					Settings* host_settings;

					bool need_init;

					Base(asIScriptEngine* engine, Settings* settings);
				};

				class Comp : public Settings, public Base {
				public:
					Comp(asIScriptEngine* engine, Settings* settings);
					bool operator()(const aatc::common::primunion& lhs, const aatc::common::primunion& rhs) const;
				};
				class Equals : public Settings, public Base {
				public:
					Equals(asIScriptEngine* engine, Settings* settings);
					bool operator()(const aatc::common::primunion& lhs, const aatc::common::primunion& rhs) const;
				};
				class Hash : public Settings, public Base {
				public:
					Hash(asIScriptEngine* engine, Settings* settings);
					config::t::hash operator()(const aatc::common::primunion& a) const;
				};
			};//namespace containerfunctor_map



			namespace autoregister {
				template<template<typename T_content> typename tempspec_container_template> void register_all_tempspec_basics_for_container(asIScriptEngine* engine) {
					common::RegistrationState rs(engine);

					tempspec_container_template<config::t::int8>::Register(rs, "int8");
					tempspec_container_template<config::t::int16>::Register(rs, "int16");
					tempspec_container_template<config::t::int32>::Register(rs, "int");
					tempspec_container_template<config::t::int64>::Register(rs, "int64");

					tempspec_container_template<config::t::uint8>::Register(rs, "uint8");
					tempspec_container_template<config::t::uint16>::Register(rs, "uint16");
					tempspec_container_template<config::t::uint32>::Register(rs, "uint");
					tempspec_container_template<config::t::uint64>::Register(rs, "uint64");

					tempspec_container_template<config::t::float32>::Register(rs, "float");
					tempspec_container_template<config::t::float64>::Register(rs, "double");

					tempspec_container_template<config::t::string>::Register(rs, "string");
				}
			};//namespace autoregister



		};//namespace shared
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif