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


#ifndef _includedh_aatc_container_list
#define _includedh_aatc_container_list



#include "aatc_common.hpp"
#include "aatc_container_templated_shared.hpp"
#include "aatc_container_tempspec_shared_method.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {



		namespace templated {



			class list : public shared::Containerbase <
				aatc_acit_list<void*>,
				container::listing::CONTAINER::LIST,
				container::listing::tags_of_container::list
			> {
			public:
				list(asIObjectType* objtype);
				list(const list& other);
				list& operator=(const list& other);



				list& swap(list& other);

				void push_back(void* value);
				void pop_back();

				void push_front(void* value);
				void pop_front();

				void* back();
				void* front();

				void sort(bool ascending = true);
				void sort_funcptr(common::aatc_script_Funcpointer* funcptr, bool ascending = true);

				void erase(config::t::sizetype position);
				void erase(const Iterator& position);
				config::t::sizetype erase(config::t::sizetype range_begin, config::t::sizetype range_end);
				config::t::sizetype erase(const Iterator& range_begin, const Iterator& range_end);

				config::t::sizetype erase_value(void* value, bool all = false);

				Iterator find(void* value);
				void insert(const Iterator& position, void* value);
			};



		};//namespace templated
		namespace tempspec {



			template<typename T_content> class list : public shared::Containerbase <
				aatc_acit_list<T_content>,
				T_content,
				container::listing::CONTAINER::LIST,
				container::listing::tags_of_container::list
			> {
			public:
				list() {}
				list(const list& other):
					Containerbase(other)
				{}
				list& list::operator=(const list& other) { Containerbase::operator=(other); return *this; }
				list& swap(list& other) { shared::method::swap(this, other); return *this; }



				void push_back(const T_content& value) { shared::method::native::push_back(this, value); }
				void pop_back() { shared::method::native::pop_back(this); }

				void push_front(const T_content& value) { shared::method::native::push_front(this, value); }
				void pop_front() { shared::method::native::pop_front(this); }

				T_content& back() { return shared::method::native::back(this); }
				T_content& front() { return shared::method::native::front(this); }

				void insert(config::t::sizetype position, const T_content& value) { shared::method::genericcc::insert_position_before_linear(this, position, value); }
				void insert(const Iterator& position, const T_content& value){ shared::method::native::insert_iterator(this, position, value); }

				void erase(config::t::sizetype position) { shared::method::genericcc::erase_position_linear(this, position); }
				void erase(const Iterator& position) { shared::method::native::erase_iterator(this, position); }
				config::t::sizetype erase(const Iterator& range_begin, const Iterator& range_end) { return shared::method::native::erase_iterator_range(this, range_begin, range_end); }
				config::t::sizetype erase(config::t::sizetype position_range_begin, config::t::sizetype position_range_end) { return shared::method::genericcc::erase_position_range_linear(this, position_range_begin, position_range_end); }

				config::t::sizetype erase_value(const T_content& value, bool all = false) { shared::method::genericcc::erase_value(this, value, all); }

				Iterator find(const T_content& value) { return shared::method::genericcc::find_iterator(this, value); }

				void sort(bool ascending = true) { shared::method::genericcc::sort(this, ascending); }



				static void Register(common::RegistrationState& rs, const char* n_content) {
					using namespace tempspec::shared;

					register_containerbase<list>(rs, config::scriptname::container::list, n_content);



					register_method::swap<list>(rs);

					register_method::native::push_back<list>(rs);
					register_method::native::pop_back<list>(rs);

					register_method::native::push_front<list>(rs);
					register_method::native::pop_front<list>(rs);

					register_method::native::back<list>(rs);
					register_method::native::front<list>(rs);

					register_method::genericcc::insert_position_before_linear<list>(rs);
					register_method::native::insert_iterator<list>(rs);

					register_method::genericcc::erase_position_linear<list>(rs);
					register_method::native::erase_iterator<list>(rs);
					register_method::native::erase_iterator_range<list>(rs);
					register_method::genericcc::erase_position_range_linear<list>(rs);

					register_method::genericcc::erase_value<list>(rs);

					register_method::native::sort<list>(rs);

					register_method::genericcc::find_iterator<list>(rs);
				}
			};



		};//namespace tempspec
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif