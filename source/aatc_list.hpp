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

#include "aatc_shared_template.hpp"
#include "aatc_container_traits.hpp"
//#include "aatc_container_switches.hpp"




BEGIN_AS_NAMESPACE



/*!\brief Actual class used for templates defined in script.*/
class aatc_container_list_template : public aatc_container_shared_1tp_template<aatc_acit_list<void*>, aatc_CONTAINERTYPE::LIST>{
public:
	//typedef aatc_container_shared_1tp_template<aatc_acit_list<void*>, aatc_CONTAINERTYPE::LIST> bcs;
	typedef aatc_acit_list<void*> bt;

	aatc_container_list_template(asIScriptEngine* _engine, asIObjectType* _objtype);
	aatc_container_list_template(const aatc_container_list_template& other);
	~aatc_container_list_template();

	static aatc_container_list_template* Factory(asIObjectType* _objtype);
	static aatc_container_list_template* Factory_copy(asIObjectType* _objtype, const aatc_container_list_template& other);
	aatc_container_list_template& operator=(const aatc_container_list_template& other);
	aatc_container_list_template& Swap(aatc_container_list_template& other);


	//doxygen skip
#ifndef DOXYGEN_SHOULD_SKIP_THIS

	//removing with just remove would not release handles
	class scriptfunctor_removeif_handlemode{
	public:
		aatc_container_list_template* host;
		void* target_handle;
		int removals;

		scriptfunctor_removeif_handlemode();
		bool operator()(void* handle);
	};
	class scriptfunctor_removeif_objectmode{
	public:
		aatc_container_list_template* host;
		asIScriptContext* cc;
		asIScriptFunction* func;
		void* target;
		int removals;
		bool func_is_equals;
		bool handlemode;

		scriptfunctor_removeif_objectmode();
		bool operator()(void* obj);
	};

#endif
	//doxygen skip

	int Remove(void* target);
};







END_AS_NAMESPACE

#endif