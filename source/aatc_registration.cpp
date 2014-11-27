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



#include "aatc_common.hpp"
#include "aatc_vector.hpp"
#include "aatc_list.hpp"
#include "aatc_set.hpp"
#include "aatc_unordered_set.hpp"
//#include "aatc_map.hpp"



BEGIN_AS_NAMESPACE


template<class T, bool GOTFUNC_EQUALS, bool GOTFUNC_LESS, bool GOTFUNC_HASH> void aatc_reghelp_tempspec_all(asIScriptEngine* engine, const char* n_content){
	aatc_register_container_tempspec_vector<T, GOTFUNC_EQUALS, GOTFUNC_LESS, GOTFUNC_HASH>(engine, n_content);
	aatc_register_container_tempspec_list<T, GOTFUNC_EQUALS, GOTFUNC_LESS, GOTFUNC_HASH>(engine, n_content);
	aatc_register_container_tempspec_set<T, GOTFUNC_EQUALS, GOTFUNC_LESS, GOTFUNC_HASH>(engine, n_content);
	if(GOTFUNC_HASH){
		aatc_register_container_tempspec_unordered_set<T, GOTFUNC_EQUALS, GOTFUNC_LESS, GOTFUNC_HASH>(engine, n_content);
	}
}

//aatc_type_uint64 aatc_hashfunc_djb2_aswrapper(const aatc_type_string& a){return aatc_hashfunc_djb2(a);}




void aatc_Initializer::Go(){
	//init this guy here
	aatc_primunion_defaultvalue.i64 = 0;

	engine->SetUserData(new aatc_engine_level_storage(engine), aatc_engine_userdata_id);
	engine->SetEngineUserDataCleanupCallback(aatc_engine_cleanup, aatc_engine_userdata_id);

	#if aatc_ENABLE_REGISTER_TYPEDEF_HASH_TYPE
		engine->RegisterTypedef(aatc_hash_type_scriptname, aatc_hash_type_scriptname_actual);
	#endif

	{
		int r = 0;
		char textbuf[1000];
		{//register aatc_script_Funcpointer
			const char* n_funcpointer = aatc_name_script_funcpointer;

			r = engine->RegisterObjectType(n_funcpointer, 0, asOBJ_REF); assert(r >= 0);

			sprintf_s(textbuf,1000, "%s@ f()", n_funcpointer);

			r = engine->RegisterObjectBehaviour(n_funcpointer, asBEHAVE_FACTORY, textbuf, asFUNCTIONPR(aatc_script_Funcpointer::Factory, (), aatc_script_Funcpointer*), asCALL_CDECL); assert(r >= 0);
			r = engine->RegisterObjectBehaviour(n_funcpointer, asBEHAVE_ADDREF, "void f()", asMETHOD(aatc_script_Funcpointer, refcount_Add), asCALL_THISCALL); assert(r >= 0);
			r = engine->RegisterObjectBehaviour(n_funcpointer, asBEHAVE_RELEASE, "void f()", asMETHOD(aatc_script_Funcpointer, refcount_Release), asCALL_THISCALL); assert(r >= 0);

			r = engine->RegisterObjectProperty(n_funcpointer, "bool ready", asOFFSET(aatc_script_Funcpointer, ready)); assert(r >= 0);
			r = engine->RegisterObjectProperty(n_funcpointer, "bool is_thiscall", asOFFSET(aatc_script_Funcpointer, is_thiscall)); assert(r >= 0);
			r = engine->RegisterObjectProperty(n_funcpointer, "string funcname", asOFFSET(aatc_script_Funcpointer, funcname)); assert(r >= 0);

			r = engine->RegisterObjectMethod(n_funcpointer, "bool Set(string)", asMETHODPR(aatc_script_Funcpointer, Set, (aatc_type_string), bool), asCALL_THISCALL); assert(r >= 0);
			r = engine->RegisterObjectMethod(n_funcpointer, "bool Set(string,?&in)", asMETHODPR(aatc_script_Funcpointer, Set, (aatc_type_string, void*, int), bool), asCALL_THISCALL); assert(r >= 0);
			r = engine->RegisterObjectMethod(n_funcpointer, "void Call()", asMETHOD(aatc_script_Funcpointer, scriptsidecall_CallVoid), asCALL_THISCALL); assert(r >= 0);
		}
		{//register hash functions

			sprintf_s(textbuf, 1000, "%s aatc_Hashfunc_djb2(string &in)", aatc_hash_type_scriptname_actual);
			r = engine->RegisterGlobalFunction(textbuf, asFUNCTION(aatc_hashfunc_djb2), asCALL_CDECL); assert(r >= 0);
			//r = engine->RegisterGlobalFunction("uint64 aatc_Hashfunc_djb2(string &in)", asFUNCTION(aatc_hashfunc_djb2_aswrapper), asCALL_CDECL); assert(r >= 0);
		}
	}

	if (include_container[aatc_CONTAINERTYPE::LIST]){ aatc_register_container<aatc_CONTAINERTYPE::LIST>(engine, this); }
	if (include_container[aatc_CONTAINERTYPE::VECTOR]){ aatc_register_container<aatc_CONTAINERTYPE::VECTOR>(engine, this); }
	if(include_container[aatc_CONTAINERTYPE::SET]){ aatc_register_container<aatc_CONTAINERTYPE::SET>(engine, this); }
	if(include_container[aatc_CONTAINERTYPE::UNORDERED_SET]){ aatc_register_container<aatc_CONTAINERTYPE::UNORDERED_SET>(engine, this); }
	if(include_container[aatc_CONTAINERTYPE::MAP]){ aatc_register_container<aatc_CONTAINERTYPE::MAP>(engine, this); }
	if(include_container[aatc_CONTAINERTYPE::UNORDERED_MAP]){ aatc_register_container<aatc_CONTAINERTYPE::UNORDERED_MAP>(engine, this); }


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