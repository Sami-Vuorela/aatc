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


#ifndef _includedh_aatc_hash
#define _includedh_aatc_hash

#include "aatc.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace hash {



		/*!\brief The functor that unordered containers use for hashing.

			You should specialize this for your c++ classes before making
			tempspecs of unordered containers of them.
		*/
		template<class T> class hashfunctor {
		public:
			aatc_hash_type operator()(const T& a) const {
				return (aatc_hash_type)a;
			}
		};
		template<> class hashfunctor<aatc_type_float32> {
		public:
			aatc_hash_type operator()(const aatc_type_float32& a) const;
		};
		template<> class hashfunctor<aatc_type_float64> {
		public:
			aatc_hash_type operator()(const aatc_type_float64& a) const;
		};
		template<> class hashfunctor<aatc_type_string> {
		public:
			aatc_hash_type operator()(const aatc_type_string& a) const;
		};



		namespace detail {
			template<typename T> aatc_hash_type function_wrapping_a_functor(const T& me) {
				hashfunctor<T> functor;
				return functor(me);
			}
		};

		namespace hashfunc {
			aatc_hash_type djb2(const aatc_type_string& a);
		};



		/*
			Use this to register your c++ classes for hashing in containers that don't have a tempspec available.
			Because you can't register functors with RegisterObjectMethod.
		*/

		template<typename T_your_cpp_type> void Register_aatc_func_hash_value(asIScriptEngine* engine, const char* name_your_type_in_script) {
			char textbuf[1000];
			sprintf_s(textbuf, 1000, "%s %s()", aatc_hash_type_scriptname_actual, config::scriptname::method::content::hash);
			int error = engine->RegisterObjectMethod(name_your_type_in_script, textbuf, asFUNCTION(detail::function_wrapping_a_functor<T_your_cpp_type>), asCALL_CDECL_OBJLAST); assert(error >= 0);
		}



	};//namespace hash
};//namespace aatc
END_AS_NAMESPACE



#endif