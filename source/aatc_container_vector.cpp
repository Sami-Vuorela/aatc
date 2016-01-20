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



#include "aatc_container_vector.hpp"

#include "aatc_container_listing.hpp"
#include "aatc_container_templated_shared_method.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace templated {



			vector::vector(asIObjectType* _objtype) :
				Containerbase(_objtype->GetEngine(), _objtype)
			{}
			vector::vector(const vector& other) :
				Containerbase(other.engine, other.objtype_container)
			{
				(*this) = other;
			}

			vector& vector::operator=(const vector& other) { Containerbase::operator=(other); return *this; }
			vector& vector::swap(vector& other) {
				shared::method::swap(this, other);
				return *this;
			}



		};//namespace templated
		namespace listing {



			template<> void register_container<CONTAINER::VECTOR>(asIScriptEngine* engine) {
				using templated::vector;

				common::RegistrationState rs(engine);

				templated::shared::register_containerbase<vector>(rs, "vector");
				vector::Iterator::Register(rs);



				templated::shared::register_method::swap<vector>(rs);
				templated::shared::register_method::push_back<vector>(rs);
				templated::shared::register_method::back<vector>(rs);
			}
			template<> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template<CONTAINER::VECTOR>(enginestorage::template_specific_storage* tss) {
				common::container_operations_bitmask_type mask = 0;
				
				if (!tss->func_cmp) {
					mask |= common::aatc_CONTAINER_OPERATION::SORT;
				}
				if (!tss->func_equals) {
					mask |= common::aatc_CONTAINER_OPERATION::COUNT;
				}
				
				return mask;
			}



		};//namespace listing



	};//namespace container
};//namespace aatc

END_AS_NAMESPACE