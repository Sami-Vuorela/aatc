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



#include "aatc_container_map.hpp"

#include "aatc_container_listing.hpp"
#include "aatc_container_mapped_templated_shared_method.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace mapped {
			namespace templated {



				map::map(asIObjectType* _objtype) :
					Containerbase(_objtype->GetEngine(), _objtype)
				{}
				map::map(const map& other) :
					Containerbase(other.engine, other.objtype_container)
				{
					(*this) = other;
				}
				map& map::operator=(const map& other) { Containerbase::operator=(other); return *this; }

				map& map::swap(map& other) {
					shared::method::swap(this, other);
					return *this;
				}

				void map::insert(void* key, void* value) {
					shared::method::insert(this, key, value);
				}



			};//namespace templated
		};//namespace mapped
		namespace listing {



			template<> void register_container<CONTAINER::MAP>(asIScriptEngine* engine) {
				common::RegistrationState rs(engine);

				{
					using mapped::templated::map;
					using namespace mapped::templated::shared;

					register_containerbase<map>(rs, config::scriptname::container::map);



					register_method::swap<map>(rs);

					register_method::insert<map>(rs);
				}
			}
			template<> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template<CONTAINER::MAP>(enginestorage::template_specific_storage* tss) {
				common::container_operations_bitmask_type mask = 0;

				if (!tss->func_cmp) {
					mask |= common::aatc_CONTAINER_OPERATION::ERASE_VALUE;
					mask |= common::aatc_CONTAINER_OPERATION::INSERT;
					mask |= common::aatc_CONTAINER_OPERATION::CONTAINS_NATIVE;
					mask |= common::aatc_CONTAINER_OPERATION::FIND;
				}
				
				return mask;
			}



		};//namespace listing



	};//namespace container
};//namespace aatc

END_AS_NAMESPACE