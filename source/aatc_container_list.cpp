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



#include "aatc_container_list.hpp"

#include "aatc_container_listing.hpp"
#include "aatc_container_templated_shared_method.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace listing {



			const char* container::listing::tags_of_container::list::scriptname_container = config::scriptname::container::list;



		}//namespace listing
		namespace templated {



			list::list(asITypeInfo* _objtype) :
				Containerbase(_objtype->GetEngine(), _objtype)
			{}
			list::list(const list& other) :
				Containerbase(other.engine, other.objtype_container)
			{
				(*this) = other;
			}
			list& list::operator=(const list& other) { Containerbase::operator=(other); return *this; }

			list& list::swap(list& other) {
				shared::method::swap(this, other);
				return *this;
			}

			void list::push_back(void* value) { shared::method::native::push_back(this, value); }
			void list::pop_back() { shared::method::native::pop_back(this); }
			void list::push_front(void* value) { shared::method::native::push_front(this, value); }
			void list::pop_front() { shared::method::native::pop_front(this); }
			void* list::back() { return shared::method::native::back(this); }
			void* list::front() { return shared::method::native::front(this); }

			void list::sort(bool ascending) { shared::method::native::sort(this, ascending); }
			void list::sort_funcptr(common::script_Funcpointer* funcptr, bool ascending) { shared::method::native::sort_funcptr(this,funcptr, ascending); }

			void list::erase(config::t::sizetype position) { shared::method::genericcc::erase_position_linear(this, position); }
			void list::erase(const Iterator& position) { shared::method::native::erase_iterator(this, position); }
			config::t::sizetype list::erase(config::t::sizetype range_begin, config::t::sizetype range_end) { return shared::method::genericcc::erase_position_range_linear(this, range_begin, range_end); }
			config::t::sizetype list::erase(const Iterator& range_begin, const Iterator& range_end) { return shared::method::native::erase_iterator_range(this, range_begin, range_end); }

			config::t::sizetype list::erase_value(void* value, bool all) { return shared::method::genericcc::erase_value(this, value, all); }

			list::Iterator list::find(void* value) { return shared::method::genericcc::find_iterator(this, value); }

			void list::insert(const Iterator& position, void* value) { shared::method::native::insert_iterator(this, position, value); }

			bool list::contains(void* value) { return shared::method::genericcc::contains(this, value); }
			config::t::sizetype list::count(void* value) { return shared::method::genericcc::count(this, value); }



		};//namespace templated
		namespace listing {



			template<> void register_container<CONTAINER::LIST>(asIScriptEngine* engine) {
				common::RegistrationState rs(engine);

				{
					using templated::list;
					using namespace templated::shared;

					register_containerbase<list>(rs);



					register_method::swap<list>(rs);

					register_method::native::push_back<list>(rs);
					register_method::native::pop_back<list>(rs);

					register_method::native::push_front<list>(rs);
					register_method::native::pop_front<list>(rs);

					register_method::native::back<list>(rs);
					register_method::native::front<list>(rs);

					register_method::native::sort<list>(rs);

					register_method::native::erase_iterator<list>(rs);
					register_method::genericcc::erase_position_linear<list>(rs);
					register_method::native::erase_iterator_range<list>(rs);
					register_method::genericcc::erase_position_range_linear<list>(rs);

					register_method::genericcc::erase_value<list>(rs);

					register_method::genericcc::find_iterator<list>(rs);

					register_method::native::insert_iterator<list>(rs);

					register_method::genericcc::contains<list>(rs);
					register_method::genericcc::count<list>(rs);
				}

				container::shared::autoregister::register_all_tempspec_basics_for_container<tempspec::list>(engine);
			}
			template<> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template<CONTAINER::LIST>(enginestorage::template_specific_storage* tss) {
				common::container_operations_bitmask_type mask = 0;
				
				if (!tss->func_cmp) {
					mask |= common::CONTAINER_OPERATION::SORT;
				}
				if (!tss->func_equals) {
					mask |= common::CONTAINER_OPERATION::COUNT;
				}
				
				return mask;
			}



		};//namespace listing



	};//namespace container
};//namespace aatc

END_AS_NAMESPACE