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


#ifndef _includedh_aatc_container_unordered_set
#define _includedh_aatc_container_unordered_set



#include "aatc_common.hpp"
#include "aatc_container_templated_shared.hpp"
#include "aatc_container_tempspec_shared_method.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {



		namespace templated {



			namespace detail {
				namespace container_native_with_functors {
					typedef aatc_acit_unordered_set<
						void*,
						container::shared::containerfunctor::Hash,
						container::shared::containerfunctor::Equals
					> unordered_set;
				};
			};//namespace detail



			class unordered_set : public shared::Containerbase <
				detail::container_native_with_functors::unordered_set,
				container::listing::CONTAINER::UNORDERED_SET,
				container::listing::tags_of_container::unordered_set,
				shared::base_container_wrapper::Hashed<
					detail::container_native_with_functors::unordered_set,
					container::shared::containerfunctor::Hash,
					container::shared::containerfunctor::Equals
				>
			> {
			public:
				unordered_set(asIObjectType* objtype);
				unordered_set(const unordered_set& other);
				unordered_set& operator=(const unordered_set& other);
				unordered_set& swap(unordered_set& other);



				void insert(void* value);

				bool erase(void* value);
				void erase(const Iterator& position);
				config::t::sizetype erase(const Iterator& range_begin, const Iterator& range_end);

				Iterator find(void* value);
				bool contains(void* value);
			};



		};//namespace templated
		namespace tempspec {



			template<typename T_content> class unordered_set : public shared::Containerbase <
				aatc_acit_unordered_set<T_content>,
				T_content,
				container::listing::CONTAINER::UNORDERED_SET,
				container::listing::tags_of_container::unordered_set
			> {
			public:
				unordered_set() {}
				unordered_set(const unordered_set& other):
					Containerbase(other)
				{}
				unordered_set& unordered_set::operator=(const unordered_set& other) { Containerbase::operator=(other); return *this; }
				unordered_set& swap(unordered_set& other) { shared::method::swap(this, other); return *this; }



				void insert(const T_content& value) { shared::method::native::insert_value(this, value); }

				bool erase(const T_content& value) { return shared::method::native::erase_value(this, value); }
				void erase(const Iterator& position) { shared::method::native::erase_iterator(this, position); }
				config::t::sizetype erase(const Iterator& range_begin, const Iterator& range_end) { return shared::method::native::erase_iterator_range(this, range_begin, range_end); }

				Iterator find(const T_content& value) { return shared::method::native::find_iterator(this, value); }
				bool contains(const T_content& value) { return shared::method::native::contains(this, value); }



				static void Register(common::RegistrationState& rs, const char* n_content) {
					using namespace tempspec::shared;

					register_containerbase<unordered_set>(rs, n_content);
					register_method::swap<unordered_set>(rs);



					register_method::native::insert_value<unordered_set>(rs);

					register_method::native::erase_value<unordered_set>(rs);
					register_method::native::erase_iterator<unordered_set>(rs);
					register_method::native::erase_iterator_range<unordered_set>(rs);

					register_method::genericcc::find_iterator<unordered_set>(rs);
					register_method::native::contains<unordered_set>(rs);
				}
			};



		};//namespace tempspec
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif