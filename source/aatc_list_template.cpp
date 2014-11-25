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
#include "aatc_list.hpp"



BEGIN_AS_NAMESPACE

/*!\brief Actual class used for templates defined in script.*/
class aatc_container_list_template : public aatc_container_shared_1tp_template<aatc_acit_list<void*>, aatc_CONTAINERTYPE::LIST>{
public:
	//typedef aatc_container_shared_1tp_template<aatc_acit_list<void*>, aatc_CONTAINERTYPE::LIST> bcs;
	typedef aatc_acit_list<void*> bt;

	aatc_container_list_template(asIScriptEngine* _engine, asIObjectType* _objtype) :
		aatc_container_shared_1tp_template(_engine,_objtype)
	{}
	aatc_container_list_template(const aatc_container_list_template& other) :
		aatc_container_shared_1tp_template(other.engine, other.objtype_container)
	{(*this) = other;}
	~aatc_container_list_template(){}

	static aatc_container_list_template* Factory(asIObjectType* _objtype){
		return new aatc_container_list_template(asGetActiveContext()->GetEngine(), _objtype);
	}
	static aatc_container_list_template* Factory_copy(asIObjectType* _objtype, const aatc_container_list_template& other){
		return new aatc_container_list_template(other);
	}
	aatc_container_list_template& operator=(const aatc_container_list_template& other){ aatc_container_shared_1tp_template::operator=(other); return *this; }
	aatc_container_list_template& Swap(aatc_container_list_template& other){ aatc_container_shared_1tp_template::swap(other); return *this; }


//doxygen skip
#ifndef DOXYGEN_SHOULD_SKIP_THIS

	//removing with just remove would not release handles
	class scriptfunctor_removeif_handlemode{
	public:
		aatc_container_list_template* host;
		void* target_handle;
		int removals;

		scriptfunctor_removeif_handlemode() :
			removals(0)
		{}
		bool operator()(void* handle){
			if (handle == target_handle){
				host->ReleaseHandle(handle);
				return 1;
			}
			return 0;
		}
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

		scriptfunctor_removeif_objectmode() :
			removals(0)
		{}
		bool operator()(void* obj){
			bool rekt;
			if (func_is_equals){
				cc->Prepare(func);
				cc->SetObject(target);
				cc->SetArgObject(0, obj);
				cc->Execute();
				rekt = cc->GetReturnByte() != 0;
			}else{//func cmp
				cc->Prepare(func);
				cc->SetObject(target);
				cc->SetArgObject(0, obj);
				cc->Execute();
				rekt = cc->GetReturnDWord() == 0;
			}
			if (rekt){
				if (handlemode){
					host->ReleaseHandle(obj);
				}else{
					host->engine->ReleaseScriptObject(obj, host->objtype_content);
				}
			}
			return rekt;
		}
	};

#endif
//doxygen skip

	int Remove(void* target){
		if(handlemode_directcomp){
			void* handle_target = *(void**)target;

			scriptfunctor_removeif_handlemode functor;
			functor.host = this;
			functor.target_handle = handle_target;
			bt::remove_if(functor);

			return functor.removals;
		}

		aatc_errorcheck_container_missingfunctions_operation_retnull(aatc_CONTAINER_OPERATION::REMOVE, objtype_container->GetName(), objtype_content->GetName(), "remove")

		scriptfunctor_removeif_objectmode functor;
			functor.host = this;
			functor.cc = els->contextcache_Get();
			functor.handlemode = handlemode;
			if (handlemode){
				functor.target = *(void**)target;
			}else{
				functor.target = target;
			}
			if (func_equals){
				functor.func_is_equals = 1;
				functor.func = func_equals;
			}else{
				functor.func = func_cmp;
			}
		bt::remove_if(functor);
		els->contextcache_Return(functor.cc);

		return functor.removals;
	}
};



template<> void aatc_register_container<aatc_CONTAINERTYPE::LIST>(asIScriptEngine* engine, aatc_Initializer* initializer){
	int r = 0;
	char textbuf[1000];

	char n_container_T[1000];
	char n_iterator[1000];
	sprintf_s(n_container_T,1000, "%s<T>", aatc_name_script_container_list);
	sprintf_s(n_iterator, 1000, "%s%s", aatc_name_script_container_list, aatc_name_script_iterator);
	char n_iterator_TT[1000];
	sprintf_s(n_iterator_TT, 1000, "%s<T>", n_iterator);

	aatc_container_shared_1tp_template_Register<aatc_container_list_template,
		aatc_container_list_needfunc_BACK_WRITE,
		aatc_container_list_needfunc_BACK_READ,
		aatc_container_list_needfunc_FRONT_WRITE,
		aatc_container_list_needfunc_FRONT_READ,
		aatc_container_list_needfunc_ERASE_POSITION,
		aatc_container_list_needfunc_ERASE_VALUE,
		aatc_container_list_needfunc_OP_INDEX,
		aatc_container_list_needfunc_RESERVE,
		aatc_container_list_needfunc_INSERT,
		aatc_container_list_needfunc_SORT_NATIVE,
		aatc_container_list_needfunc_SORT_GENERIC,
		aatc_container_list_needfunc_CONTAINS_NATIVE,
		aatc_container_list_needfunc_ERASE_GENERIC_INDEX,
		aatc_container_list_needfunc_ERASE_GENERIC_VALUE,
		aatc_container_list_needfunc_INSERT_GENERIC_INDEX
	>(engine, aatc_name_script_container_list);

	aect_iterator_shared_template<aatc_container_list_template>::Register(engine, n_iterator, n_container_T);

	{//container type specific functions
		r = engine->RegisterObjectMethod(n_container_T, "int remove(const T&in)", asMETHOD(aatc_container_list_template, Remove), asCALL_THISCALL); assert(r >= 0);
	}

	sprintf_s(textbuf, 1000, "%s %s()", n_iterator_TT, aatc_name_script_container_method_begin);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asFunctionPtr(aatc_reghelp_construct_hosted_iterator_template<aect_iterator_shared_template<aatc_container_list_template>, aatc_container_list_template*>), asCALL_CDECL_OBJLAST); assert(r >= 0);
}

template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::LIST>(aatc_template_specific_storage* tss){
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