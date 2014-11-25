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



#include "aatc_shared_map_template.hpp"



BEGIN_AS_NAMESPACE

typedef aatc_acit_unordered_map<	
	aatc_primunion,
	aatc_primunion,
	aatc_containerfunctor_map_hash,
	aatc_containerfunctor_map_equals
> aatc_acit_unordered_map_with_functors;

/*!\brief Actual class used for templates defined in script.*/
class aatc_container_unordered_map_template : public aatc_container_shared_map_template<
	aatc_acit_unordered_map_with_functors,
	aatc_CONTAINERTYPE::UNORDERED_MAP,
	aatc_shared_map_bcw_hashmap<
		aatc_acit_unordered_map_with_functors,
		aatc_containerfunctor_map_hash,
		aatc_containerfunctor_map_equals,
		aatc_CONFIG_DEFAULT_CONTAINER_UNORDERED_MAP_DEFAULTBUCKETCOUNT
	>
>{
public:
	typedef aatc_acit_unordered_map_with_functors bt;

	aatc_container_unordered_map_template(asIScriptEngine* _engine, asIObjectType* _objtype) :
		aatc_container_shared_map_template(_engine, _objtype)
	{}
	aatc_container_unordered_map_template(const aatc_container_unordered_map_template& other) :
		aatc_container_shared_map_template(other.engine, other.objtype_container)
	{
		(*this) = other;
	}
	~aatc_container_unordered_map_template(){}

	static aatc_container_unordered_map_template* Factory(asIObjectType* _objtype_container){
		asIScriptEngine* engine = asGetActiveContext()->GetEngine();
		return new aatc_container_unordered_map_template(engine, _objtype_container);
	}
	static aatc_container_unordered_map_template* Factory_copy(asIObjectType* _objtype, const aatc_container_unordered_map_template& other){
		return new aatc_container_unordered_map_template(other);
	}
	aatc_container_unordered_map_template& operator=(const aatc_container_unordered_map_template& other){ aatc_container_shared_map_template::operator=(other); return *this; }
	aatc_container_unordered_map_template& Swap(aatc_container_unordered_map_template& other){ aatc_container_shared_map_template::swap(other); return *this; }
};



template<> void aatc_register_container<aatc_CONTAINERTYPE::UNORDERED_MAP>(asIScriptEngine* engine, aatc_Initializer* initializer){
	int r = 0;
	char textbuf[1000];
	
	char n_container_T[1000];
	char n_iterator[1000];
	sprintf_s(n_container_T, 1000, "%s<T_key,T_value>", aatc_name_script_container_unordered_map);
	sprintf_s(n_iterator, 1000, "%s%s", aatc_name_script_container_unordered_map, aatc_name_script_iterator);
	char n_iterator_TT[1000];
	sprintf_s(n_iterator_TT, 1000, "%s<T_key,T_value>", n_iterator);

	aatc_container_shared_map_template_Register<aatc_container_unordered_map_template>(engine, aatc_name_script_container_unordered_map);

	aect_iterator_shared_map_template<aatc_container_unordered_map_template>::Register(engine, n_iterator, n_container_T);

	sprintf_s(textbuf, 1000, "%s %s()", n_iterator_TT, aatc_name_script_container_method_begin);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asFunctionPtr(aatc_reghelp_construct_hosted_iterator_map_template<aect_iterator_shared_map_template<aatc_container_unordered_map_template>, aatc_container_unordered_map_template*>), asCALL_CDECL_OBJLAST); assert(r >= 0);
}

template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::UNORDERED_MAP>(aatc_template_specific_storage* tss){
	aatc_container_operations_bitmask_type mask = 0;

	if((!tss->func_hash) || (!tss->func_equals)){
		mask |= aatc_CONTAINER_OPERATION::ERASE_VALUE;
		mask |= aatc_CONTAINER_OPERATION::INSERT;
		mask |= aatc_CONTAINER_OPERATION::CONTAINS_NATIVE;
		mask |= aatc_CONTAINER_OPERATION::FIND;
	}
	//if(!tss->func_equals){
	//	mask |= aatc_CONTAINER_OPERATION::COUNT;
	//}

	return mask;
}
END_AS_NAMESPACE