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
		vector::~vector() {

		}

		vector* vector::Factory(asIObjectType* _objtype) {
			return new vector(_objtype);
		}
		vector* vector::Factory_copy(asIObjectType* _objtype, const vector& other){
			return new vector(other);
		}

		vector& vector::operator=(const vector& other) {

			return *this;
		}
		vector& vector::Swap(vector& other) {
			
			return *this;
		}



//template<> void aatc_register_container<aatc_CONTAINERTYPE::VECTOR>(asIScriptEngine* engine, aatc_Initializer* initializer) {
//	aatc_container_shared_1tp_template_Register<aatc_container_vector_template, aatc_container_traits_vector>(engine, aatc_name_script_container_vector);
//}
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