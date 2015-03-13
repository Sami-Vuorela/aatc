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



#include "aatc_unordered_map.hpp"



BEGIN_AS_NAMESPACE


aatc_container_unordered_map_template::aatc_container_unordered_map_template(asIScriptEngine* _engine, asIObjectType* _objtype) :
	aatc_container_shared_map_template(_engine, _objtype)
{}
aatc_container_unordered_map_template::aatc_container_unordered_map_template(const aatc_container_unordered_map_template& other) :
	aatc_container_shared_map_template(other.engine, other.objtype_container)
{
	(*this) = other;
}
aatc_container_unordered_map_template::~aatc_container_unordered_map_template(){}

aatc_container_unordered_map_template* aatc_container_unordered_map_template::Factory(asIObjectType* _objtype_container){
	asIScriptEngine* engine = _objtype_container->GetEngine();
	return new aatc_container_unordered_map_template(engine, _objtype_container);
}
aatc_container_unordered_map_template* aatc_container_unordered_map_template::Factory_copy(asIObjectType* _objtype, const aatc_container_unordered_map_template& other){
	return new aatc_container_unordered_map_template(other);
}
aatc_container_unordered_map_template& aatc_container_unordered_map_template::operator=(const aatc_container_unordered_map_template& other){ aatc_container_shared_map_template::operator=(other); return *this; }
aatc_container_unordered_map_template& aatc_container_unordered_map_template::Swap(aatc_container_unordered_map_template& other){ aatc_container_shared_map_template::swap(other); return *this; }



template<> void aatc_register_container<aatc_CONTAINERTYPE::UNORDERED_MAP>(asIScriptEngine* engine, aatc_Initializer* initializer){
	aatc_container_shared_map_template_Register<aatc_container_unordered_map_template>(engine, aatc_name_script_container_unordered_map);
}

template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::UNORDERED_MAP>(aatc_template_specific_storage* tss){
	aatc_container_operations_bitmask_type mask = 0;

	if((!tss->func_hash) || (!tss->func_equals)){
		mask |= aatc_CONTAINER_OPERATION::ERASE_VALUE;
		mask |= aatc_CONTAINER_OPERATION::INSERT;
		mask |= aatc_CONTAINER_OPERATION::CONTAINS_NATIVE;
		mask |= aatc_CONTAINER_OPERATION::FIND;
	}

	return mask;
}



END_AS_NAMESPACE