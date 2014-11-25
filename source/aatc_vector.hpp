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


#ifndef _includedh_aatc_container_vector
#define _includedh_aatc_container_vector

#include "aatc_shared_tempspec.hpp"



BEGIN_AS_NAMESPACE

#define aatc_container_vector_needfunc_BACK_WRITE 1
#define aatc_container_vector_needfunc_BACK_READ 1
#define aatc_container_vector_needfunc_FRONT_WRITE 0
#define aatc_container_vector_needfunc_FRONT_READ 1
#define aatc_container_vector_needfunc_ERASE_POSITION 1
#define aatc_container_vector_needfunc_ERASE_VALUE 0
#define aatc_container_vector_needfunc_OP_INDEX 1
#define aatc_container_vector_needfunc_RESERVE 1
#define aatc_container_vector_needfunc_INSERT 0
#define aatc_container_vector_needfunc_SORT_NATIVE 0
#define aatc_container_vector_needfunc_SORT_GENERIC 1
#define aatc_container_vector_needfunc_CONTAINS_NATIVE 0
#define aatc_container_vector_needfunc_ERASE_GENERIC_INDEX aatc_Y
#define aatc_container_vector_needfunc_ERASE_GENERIC_VALUE 1
#define aatc_container_vector_needfunc_INSERT_GENERIC_INDEX aatc_Y

/*!\brief Actual class used for template specializations defined in c++.*/
template<class T> class aatc_container_vector_tempspec : public aatc_container_shared_1tp_tempspec<aatc_acit_vector<T>, T>{
public:
	aatc_container_vector_tempspec(){}
	aatc_container_vector_tempspec(const aatc_container_vector_tempspec& other) :
		aatc_container_shared_1tp_tempspec(other)
	{}
	~aatc_container_vector_tempspec(){}

	static aatc_container_vector_tempspec* Factory(){return new aatc_container_vector_tempspec();}
	static aatc_container_vector_tempspec* Factory_copy(const aatc_container_vector_tempspec& other){return new aatc_container_vector_tempspec(other);}
	aatc_container_vector_tempspec& operator=(const aatc_container_vector_tempspec& other){ aatc_container_shared_1tp_tempspec::operator=(other); return *this; }
	aatc_container_vector_tempspec& Swap(aatc_container_vector_tempspec& other){ aatc_container_shared_1tp_tempspec::swap(other); return *this; }
};



template<
	class dt_content,
	bool GOTFUNC_EQUALS = 1,
	bool GOTFUNC_LESS = 0,
	bool GOTFUNC_HASH = 1
> void aatc_register_container_tempspec_vector(asIScriptEngine* engine, const char* n_content){
	typedef aatc_container_vector_tempspec<dt_content> dt_container;


	int r = 0;
	char textbuf[1000];

	char n_containerTcontentT[1000];
	char n_iterator[1000];
	sprintf_s(n_containerTcontentT, 1000, "%s<%s>", aatc_name_script_container_vector, n_content);
	sprintf_s(n_iterator, 1000, "%s%s<%s>", aatc_name_script_container_vector, aatc_name_script_iterator, n_content);

	dt_container::staticname_container = aatc_name_script_container_vector;
	dt_container::staticname_content = n_content;

	aatc_container_shared_1tp_tempspec_Register<dt_container, dt_content, GOTFUNC_EQUALS, GOTFUNC_LESS, GOTFUNC_HASH,
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
	>(engine, aatc_name_script_container_vector, n_content);

	aect_iterator_shared_tempspec<dt_container, dt_content>::Register<aatc_Y, aatc_N>(engine, n_iterator, n_content, n_containerTcontentT);

	sprintf_s(textbuf, 1000, "%s %s()", n_iterator, aatc_name_script_container_method_begin);
	r = engine->RegisterObjectMethod(n_containerTcontentT, textbuf, asFunctionPtr(aatc_reghelp_construct_hosted<aect_iterator_shared_tempspec<dt_container, dt_content>, dt_container*>), asCALL_CDECL_OBJLAST); assert(r >= 0);

	//sprintf_s(textbuf, 1000, "void erase(%s)", aatc_name_script_sizetype);
	//r = engine->RegisterObjectMethod(n_containerTcontentT, textbuf, asMETHOD(dt_container, Erase_generic_index<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
}

END_AS_NAMESPACE

#endif