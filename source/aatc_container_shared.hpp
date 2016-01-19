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
					aatc::common::engine_level_storage* els;
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
					aatc_hash_type operator()(const void* ptr) const;
				};
			};
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
					aatc::common::engine_level_storage* els;
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
					aatc_hash_type operator()(const aatc::common::primunion& a) const;
				};
			};//namespace detail





		};//namespace shared
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif