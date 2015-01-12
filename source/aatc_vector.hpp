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

#include "aatc_shared_template.hpp"
#include "aatc_container_traits.hpp"
//#include "aatc_container_switches.hpp"



BEGIN_AS_NAMESPACE



/*!\brief Actual class used for templates defined in script.*/
class aatc_container_vector_template : public aatc_container_shared_1tp_template<aatc_acit_vector<void*>, aatc_CONTAINERTYPE::VECTOR>{
public:
	aatc_container_vector_template(asIScriptEngine* _engine, asIObjectType* _objtype);
	aatc_container_vector_template(const aatc_container_vector_template& other);
	~aatc_container_vector_template();

	static aatc_container_vector_template* Factory(asIObjectType* _objtype);
	static aatc_container_vector_template* Factory_copy(asIObjectType* _objtype, const aatc_container_vector_template& other);
	aatc_container_vector_template& operator=(const aatc_container_vector_template& other);
	aatc_container_vector_template& Swap(aatc_container_vector_template& other);
};




END_AS_NAMESPACE

#endif