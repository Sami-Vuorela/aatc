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



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {


		vector::vector(asIObjectType* _objtype) :
			containerbase(_objtype->GetEngine(), _objtype)
		{}
		vector::vector(const vector& other) :
			containerbase(other.engine, other.objtype_container)
		{
			(*this) = other;
		}

		vector& vector::operator=(const vector& other) { containerbase::operator=(other); return *this; }
		vector& vector::swap(vector& other) {
			aatc::container::shared::templated::method::swap(this, other);
			return *this;
		}

		void debug_Register_Vector(asIScriptEngine* engine, aatc_Initializer* initializer) {
			aatc::common::RegistrationState rs(engine);

			aatc::container::shared::templated::register_containerbase<vector>(rs, "vector");



			aatc::container::shared::templated::register_method::swap<vector>(rs);

			aatc::container::shared::templated::register_method::push_back<vector>(rs);
			aatc::container::shared::templated::register_method::back<vector>(rs);
		}



//
//template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::VECTOR>(aatc_template_specific_storage* tss) {
//	aatc_container_operations_bitmask_type mask = 0;
//
//	if (!tss->func_cmp) {
//		mask |= aatc_CONTAINER_OPERATION::SORT;
//	}
//	if (!tss->func_equals) {
//		mask |= aatc_CONTAINER_OPERATION::COUNT;
//	}
//
//	return mask;
//}




	};//namespace container
};//namespace aatc

END_AS_NAMESPACE