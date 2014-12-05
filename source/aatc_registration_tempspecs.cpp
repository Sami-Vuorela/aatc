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



#include "aatc_tempspecs.hpp"





BEGIN_AS_NAMESPACE



template<class T, bool GOTFUNC_EQUALS, bool GOTFUNC_LESS, bool GOTFUNC_HASH> void aatc_reghelp_tempspec_all(asIScriptEngine* engine, const char* n_content){
	aatc_register_container_tempspec_vector<T, GOTFUNC_EQUALS, GOTFUNC_LESS, GOTFUNC_HASH>(engine, n_content);
	aatc_register_container_tempspec_list<T, GOTFUNC_EQUALS, GOTFUNC_LESS, GOTFUNC_HASH>(engine, n_content);
	aatc_register_container_tempspec_set<T, GOTFUNC_EQUALS, GOTFUNC_LESS, GOTFUNC_HASH>(engine, n_content);
	if(GOTFUNC_HASH){
		aatc_register_container_tempspec_unordered_set<T, GOTFUNC_EQUALS, GOTFUNC_LESS, GOTFUNC_HASH>(engine, n_content);
	}
}



void aatc_Initializer::RegisterTempspecs(){
	#if aatc_include_primitive_native_tempspec_INT8
		aatc_reghelp_tempspec_all<aatc_type_int8,1,1,1>(engine, "int8");
	#endif
	#if aatc_include_primitive_native_tempspec_INT16
		aatc_reghelp_tempspec_all<aatc_type_int16, 1, 1, 1>(engine, "int16");
	#endif
	#if aatc_include_primitive_native_tempspec_INT32
		aatc_reghelp_tempspec_all<aatc_type_int32, 1, 1, 1>(engine, "int");
	#endif
	#if aatc_include_primitive_native_tempspec_INT64
		aatc_reghelp_tempspec_all<aatc_type_int64, 1, 1, 1>(engine, "int64");
	#endif

	#if aatc_include_primitive_native_tempspec_UINT8
		aatc_reghelp_tempspec_all<aatc_type_uint8, 1, 1, 1>(engine, "uint8");
	#endif
	#if aatc_include_primitive_native_tempspec_UINT16
		aatc_reghelp_tempspec_all<aatc_type_uint16, 1, 1, 1>(engine, "uint16");
	#endif
	#if aatc_include_primitive_native_tempspec_UINT32
		aatc_reghelp_tempspec_all<aatc_type_uint32, 1, 1, 1>(engine, "uint");
	#endif
	#if aatc_include_primitive_native_tempspec_UINT64
		aatc_reghelp_tempspec_all<aatc_type_uint64, 1, 1, 1>(engine, "uint64");
	#endif

	#if aatc_include_primitive_native_tempspec_FLOAT32
		aatc_reghelp_tempspec_all<aatc_type_float32, 1, 1, 1>(engine, "float");
	#endif
	#if aatc_include_primitive_native_tempspec_FLOAT64
		aatc_reghelp_tempspec_all<aatc_type_float64, 1, 1, 1>(engine, "double");
	#endif

	#if aatc_include_primitive_native_tempspec_STRING
		aatc_reghelp_tempspec_all<aatc_type_string, 1, 1, 1>(engine, "string");
	#endif

	#if aatc_CONFIG_USE_ASADDON_REF
		aatc_reghelp_tempspec_all<aatc_ait_ref, 1, 1, 0>(engine, "ref");
	#endif
}



END_AS_NAMESPACE