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


#ifndef _includedh_aatc_container_vector
#define _includedh_aatc_container_vector



#include "aatc_common.hpp"
#include "aatc_container_templated_shared.hpp"
#include "aatc_container_tempspec_shared_method.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {



		namespace detail {
			namespace tags_of_container {
				class vector : public shared::tagbase {
				public:
					typedef shared::tag::iterator_access_is_mutable iterator_access;
				};
			};
		};



		namespace templated {



			class vector : public shared::Containerbase <
				aatc_acit_vector<void*>,
				aatc::container::listing::CONTAINER::VECTOR,
				container::detail::tags_of_container::vector
			> {
			public:
				vector(asIObjectType* objtype);
				vector(const vector& other);
				vector& operator=(const vector& other);



				vector& swap(vector& other);

				void push_back(void* a);
				void* back();
			};



		};//namespace templated
		namespace tempspec {



			template<typename T_content> class vector : public shared::Containerbase <
				aatc_acit_vector<T_content>,
				T_content,
				container::detail::tags_of_container::vector
			> {
			public:
				vector() {}
				vector(const vector& other):
					Containerbase(other)
				{}
				vector& vector::operator=(const vector& other) { Containerbase::operator=(other); return *this; }



				vector& swap(vector& other) { shared::method::swap(this, other); return *this; }

				void push_back(const T_content& a) { shared::method::push_back(this, a); }
				T_content back() { return shared::method::back(this); }



				static void Register(common::RegistrationState& rs, const char* n_content) {
					using namespace tempspec::shared;

					register_containerbase<vector>(rs, config::scriptname::container::vector, n_content);



					register_method::swap<vector>(rs);
					register_method::push_back<vector>(rs);
					register_method::back<vector>(rs);
				}
			};



		};//namespace tempspec
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif