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



//#include "aatc_internal_lists.hpp"
#include "aatc.hpp"
#include "aatc_common.hpp"
#include "aatc_hash.hpp"
#include "aatc_enginestorage.hpp"
#include "aatc_container_listing.hpp"
#include "aatc_templatemagic.hpp"





BEGIN_AS_NAMESPACE
namespace aatc {


	template<int i, typename TT> class Initializer_tm_iterate_all_containers {
	public:
		void operator()(TT& tup) {
			if (std::get<0>(tup)->include_container[i]) {
				asIScriptEngine* engine = std::get<1>(tup);
				enginestorage::engine_level_storage* els = enginestorage::Get_ELS(engine);
				enginestorage::containertype_specific_storage& ctss = els->containertype_specific_storages[i];

				ctss.els = els;
				ctss.container_id = i;
				ctss.func_errorcheck_missing_functions_make_bitfield_for_template = container::listing::errorcheck_missing_functions_make_bitfield_for_template<i>;

				container::listing::register_container<i>(std::get<1>(tup));
			}
		}

		//static const int id = std::tuple_element<i, aatc_infos_all_tuple>::type::container_id;
		//void operator()(TT& tup) {
		//	if (std::get<0>(tup)->include_container[id]) { aatc_register_container<id>(std::get<1>(tup), std::get<0>(tup)); }
		//}
	};



	void Initializer::Go() {
		//init this guy here
		common::primunion_defaultvalue.ui64 = 0;
		common::primunion_defaultvalue.ptr = nullptr;

		engine->SetUserData(new enginestorage::engine_level_storage(engine), config::engine_userdata_id);
		engine->SetEngineUserDataCleanupCallback(common::aatc_engine_cleanup, config::engine_userdata_id);

		#if aatc_ENABLE_REGISTER_TYPEDEF_HASH_TYPE
			engine->RegisterTypedef(aatc_hash_type_scriptname, aatc_hash_type_scriptname_actual);
		#endif

		{
			int r = 0;
			char textbuf[1000];
			{//register aatc_script_Funcpointer
				using common::aatc_script_Funcpointer;

				const char* n_funcpointer = config::scriptname::funcpointer;

				r = engine->RegisterObjectType(n_funcpointer, 0, asOBJ_REF); assert(r >= 0);

				sprintf_s(textbuf, 1000, "%s@ f()", n_funcpointer);

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
				r = engine->RegisterGlobalFunction(textbuf, asFUNCTION(hash::hashfunc::djb2), asCALL_CDECL); assert(r >= 0);
			}
		}

		aatc_tm_iterator_1arg_functor<0, container::listing::CONTAINER::_COUNT - 1, Initializer_tm_iterate_all_containers, std::tuple< Initializer*, asIScriptEngine*>> f; f(std::make_tuple(this, engine));



		//RegisterTempspecs();
		//RegisterTempspecs2();
	}


};//namespace aatc
END_AS_NAMESPACE