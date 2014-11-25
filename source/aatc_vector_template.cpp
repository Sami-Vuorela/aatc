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



#include "aatc_shared_template.hpp"
#include "aatc_vector.hpp"



BEGIN_AS_NAMESPACE



/*!\brief Actual class used for templates defined in script.*/
class aatc_container_vector_template : public aatc_container_shared_1tp_template<aatc_acit_vector<void*>, aatc_CONTAINERTYPE::VECTOR>{
public:
	//typedef aatc_container_shared_1tp_template<aatc_acit_vector<void*>, aatc_CONTAINERTYPE::VECTOR> bcs;
	//typedef aatc_acit_vector<void*> bt;

	aatc_container_vector_template(asIScriptEngine* _engine, asIObjectType* _objtype) :
		aatc_container_shared_1tp_template(_engine,_objtype)
	{}
	aatc_container_vector_template(const aatc_container_vector_template& other) :
		aatc_container_shared_1tp_template(other.engine, other.objtype_container)
	{(*this) = other;}
	~aatc_container_vector_template(){}

	static aatc_container_vector_template* Factory(asIObjectType* _objtype){
		return new aatc_container_vector_template(asGetActiveContext()->GetEngine(), _objtype);
	}
	static aatc_container_vector_template* Factory_copy(asIObjectType* _objtype, const aatc_container_vector_template& other){
		return new aatc_container_vector_template(other);
	}
	aatc_container_vector_template& operator=(const aatc_container_vector_template& other){ aatc_container_shared_1tp_template::operator=(other); return *this; }
	aatc_container_vector_template& Swap(aatc_container_vector_template& other){ aatc_container_shared_1tp_template::swap(other); return *this; }
};



template<> void aatc_register_container<aatc_CONTAINERTYPE::VECTOR>(asIScriptEngine* engine, aatc_Initializer* initializer){
	int r = 0;
	char textbuf[1000];

	char n_container_T[1000];
	char n_iterator[1000];
	sprintf_s(n_container_T,1000, "%s<T>", aatc_name_script_container_vector);
	sprintf_s(n_iterator, 1000, "%s%s", aatc_name_script_container_vector, aatc_name_script_iterator);
	char n_iterator_TT[1000];
	sprintf_s(n_iterator_TT, 1000, "%s<T>", n_iterator);

	aatc_container_shared_1tp_template_Register<aatc_container_vector_template,
		aatc_container_vector_needfunc_BACK_WRITE,
		aatc_container_vector_needfunc_BACK_READ,
		aatc_container_vector_needfunc_FRONT_WRITE,
		aatc_container_vector_needfunc_FRONT_READ,
		aatc_container_vector_needfunc_ERASE_POSITION,
		aatc_container_vector_needfunc_ERASE_VALUE,
		aatc_container_vector_needfunc_OP_INDEX,
		aatc_container_vector_needfunc_RESERVE,
		aatc_container_vector_needfunc_INSERT,
		aatc_container_vector_needfunc_SORT_NATIVE,
		aatc_container_vector_needfunc_SORT_GENERIC,
		aatc_container_vector_needfunc_CONTAINS_NATIVE,
		aatc_container_vector_needfunc_ERASE_GENERIC_INDEX,
		aatc_container_vector_needfunc_ERASE_GENERIC_VALUE,
		aatc_container_vector_needfunc_INSERT_GENERIC_INDEX
	>(engine, aatc_name_script_container_vector);

	aect_iterator_shared_template<aatc_container_vector_template>::Register(engine, n_iterator, n_container_T);

	sprintf_s(textbuf, 1000, "%s %s()", n_iterator_TT, aatc_name_script_container_method_begin);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asFunctionPtr(aatc_reghelp_construct_hosted_iterator_template<aect_iterator_shared_template<aatc_container_vector_template>, aatc_container_vector_template*>), asCALL_CDECL_OBJLAST); assert(r >= 0);

	//sprintf_s(textbuf, 1000, "void erase(%s)", aatc_name_script_sizetype);
	//r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_vector_template, Erase_generic_index<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
}

template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::VECTOR>(aatc_template_specific_storage* tss){
	aatc_container_operations_bitmask_type mask = 0;

	if(!tss->func_cmp){
		mask |= aatc_CONTAINER_OPERATION::SORT;
	}
	if(!tss->func_equals){
		mask |= aatc_CONTAINER_OPERATION::COUNT;
	}

	return mask;
}
END_AS_NAMESPACE