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


#ifndef _includedh_aatc_container_tempspec_shared_method
#define _includedh_aatc_container_tempspec_shared_method



#include "aatc_container_tempspec_shared.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace tempspec {
			namespace shared {



				namespace method {

					template<typename T_container> void swap(T_container* t, T_container& other) {
						t->container.swap(other.container);

						t->safety_iteratorversion_Increment();
						other.safety_iteratorversion_Increment();
					}

					template<typename T_container> void push_back(T_container* t, typename const T_container::T_content& value) {
						t->container.push_back(value);

						t->safety_iteratorversion_Increment();
					}

					template<typename T_container> typename T_container::T_content& back(T_container* t) {
						#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
							if (t->empty()) {
								aatc::common::aatc_errorprint_container_access_empty(T_container::staticname_container.c_str(), T_container::staticname_content.c_str(), config::scriptname::method::container::back);
								return T_container::defaultvalue;
							}
						#endif

						return t->container.back();
					}


				};

				namespace register_method {

					template<typename T_container> static void swap(aatc::common::RegistrationState& rs) {
						sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "%s& %s(%s@)", rs.n_container_T, config::scriptname::method::container::swap, rs.n_container_T);
						rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::swap<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
					}

					template<typename T_container> static void push_back(aatc::common::RegistrationState& rs) {
						sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "void %s(const %s &in)", config::scriptname::method::container::push_back, rs.n_content);
						rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::push_back<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
					}
					template<typename T_container> static void pop_back(aatc::common::RegistrationState& rs) {
						sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "void %s()", config::scriptname::method::container::pop_back);
						rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::pop_back<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
					}

					template<typename T_container> static void back(aatc::common::RegistrationState& rs) {
						sprintf_s(rs.textbuf, common::RegistrationState::bufsize, "%s& %s()", rs.n_content, config::scriptname::method::container::back);
						rs.error = rs.engine->RegisterObjectMethod(rs.n_container_T, rs.textbuf, asFUNCTION(method::back<T_container>), asCALL_CDECL_OBJFIRST); assert(rs.error >= 0);
					}


				};



			};//namespace shared
		};//namespace tempspec
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif