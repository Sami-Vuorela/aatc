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


#ifndef _includedh_aatc_shared_1t_generic
#define _includedh_aatc_shared_1t_generic



#include "aatc_common.hpp"

//#include "cm/core/print.hpp"



BEGIN_AS_NAMESPACE

//doxygen skip
#ifndef DOXYGEN_SHOULD_SKIP_THIS

//we need a wrapper around the base container so that we can
//feed the constructor a fancy functor to handle internal functions that require script functions to be called

template<class T_container> class aatc_bcw_basic : public T_container{
public:
	aatc_bcw_basic(asIScriptEngine* engine, aatc_containerfunctor_Settings* settings){}
};
//template<class T_container> class aatc_bcw_1_param : public T_container{
//public:
//	aatc_bcw_comp(asIScriptEngine* _engine, aatc_container_helper_compsettings* settings) :
//		T_container(aatc_containerfunctor_comp(_engine, settings))
//	{}
//};
//template<class T_container> class aatc_bcw_hash_equals : public T_container{
//public:
//	aatc_bcw_hash_equals(asIScriptEngine* _engine, aatc_container_helper_compsettings* settings) :
//		T_container(aatc_containerfunctor_hash(_engine, settings), aatc_containerfunctor_equals(_engine, settings))
//	{}
//};

template<class T_container, class T_param1> class aatc_bcw_1_param : public T_container{
public:
	aatc_bcw_1_param(asIScriptEngine* _engine, aatc_containerfunctor_Settings* settings) :
		T_container(T_param1(_engine, settings))
	{}
};
//template<class T_container, class T_param1, class T_param2> class aatc_bcw_2_param : public T_container{
//public:
//	aatc_bcw_2_param(asIScriptEngine* _engine, aatc_containerfunctor_Settings* settings) :
//		T_container(aatc_CONFIG_DEFAULT_CONTAINER_UNORDERED_SET_defaultbucketcount,T_param1(_engine, settings), T_param2(_engine, settings))
//	{}
//};

template<class T_container, class T_param1, class T_param2,std::size_t default_bucket_count> class aatc_bcw_hashmap : public T_container{
public:
	aatc_bcw_hashmap(asIScriptEngine* _engine, aatc_containerfunctor_Settings* settings) :
		T_container(default_bucket_count,T_param1(_engine, settings), T_param2(_engine, settings))
	{}
};

#endif
//doxygen skip

//template<class T_container, int T_CONTAINERTYPEID, class basecontainerwrapperclass = aatc_bcw_basic<T_container>> class aatc_container_shared_1tp_template : public basecontainerwrapperclass, public aatc_refcounted_GC, public aatc_container_helper_compsettings{
//template<class T_container, int T_CONTAINERTYPEID, bool NEED_COMPHELPER = 0> class aatc_container_shared_1tp_template :	public std::conditional<NEED_COMPHELPER, aatc_bcw_comphelper<T_container>, aatc_bcw_basic<T_container>>::type,
//																														public aatc_refcounted_GC,
//																														public aatc_containerfunctor_Settings

/*!\brief Internal template monster

*/
template<class T_container, int T_CONTAINERTYPEID, class bcw = aatc_bcw_basic<T_container>> class aatc_container_shared_1tp_template :	public bcw,
																																		public aatc_refcounted_GC,
																																		public aatc_containerfunctor_Settings
{
public:
	typename typedef T_container::iterator iteratortype;

	//typename typedef std::conditional<NEED_COMPHELPER, aatc_bcw_comphelper<T_container>, aatc_bcw_basic<T_container>>::type bct;
	//typename typedef bcw bct;

	asIScriptEngine* engine;
	asIObjectType* objtype_container;
	asIObjectType* objtype_content;
	int astypeid_container;
	int astypeid_content;
	asDWORD typeflags_container;
	asDWORD typeflags_content;

	bool handlemode_needref;

	aatc_engine_level_storage* els;

	aatc_container_operations_bitmask_type missing_functions;

	bool directcomp_forced;

	aatc_container_shared_1tp_template(asIScriptEngine* _engine, asIObjectType* _objtype) :
		bcw(_engine, this),
		aatc_refcounted_GC(_engine),
		engine(_engine),
		objtype_container(_objtype),
		directcomp_forced(0)
	{
		handlemode_directcomp = aatc_CONFIG_DEFAULT_HANDLEMODE_DIRECTCOMP;

		objtype_content = objtype_container->GetSubType();

		astypeid_container = objtype_container->GetTypeId();
		astypeid_content = objtype_container->GetSubTypeId();

		typeflags_container = objtype_container->GetFlags();
		typeflags_content = objtype_content->GetFlags();

		objtype_container->AddRef();
		objtype_content->AddRef();

		//problem_nofunc_eq_or_cmp = 0;
		//problem_nofunc_cmp = 0;

		els = (aatc_engine_level_storage*)asGetActiveContext()->GetEngine()->GetUserData(aatc_engine_userdata_id);
		aatc_containertype_specific_storage* ctss = els->GetContainerTypeSpecificStorage(T_CONTAINERTYPEID);

		if (astypeid_content & asTYPEID_OBJHANDLE){
			handlemode = 1;
			handlemode_needref = 1;
			if (typeflags_content & asOBJ_NOCOUNT){
				handlemode_needref = 0;
			}
		}else{
			handlemode = 0;
			handlemode_directcomp = 0;
		}

		aatc_template_specific_storage* tss = ctss->GetTemplateSpecificStorage(astypeid_content);
			func_equals = tss->func_equals;
			func_cmp = tss->func_cmp;
			func_hash = tss->func_hash;

			missing_functions = tss->missing_functions;

		#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE
			if(missing_functions){
				bool bad = 1;

				#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE_USE_DIRECTCOMP
					if(handlemode){//force handlemode directcomp if anything is missing
						handlemode_directcomp = 1;
						directcomp_forced = 1;
						bad = 0;
					}
				#endif

				if(bad){
					char msg[1000];
					sprintf_s(msg, 1000, "Type '%s' has missing methods required for container '%s'.", objtype_content->GetName(), objtype_container->GetName());
					asGetActiveContext()->SetException(msg);
				}
			}
		#endif

		engine->NotifyGarbageCollectorOfNewObject(this, objtype_container);
	}
	virtual ~aatc_container_shared_1tp_template(){
		Clear();

		objtype_container->Release();
		objtype_content->Release();
	}
	void operator=(const aatc_container_shared_1tp_template& other){
		Clear();//will delete script objects or decrement handles

		T_container::operator=(other);

		if(handlemode){//increment refs for newly created handles
			for(auto it = T_container::begin(); it != T_container::end(); it++){
				engine->AddRefScriptObject(*it, objtype_content);
			}
		}else{//value objects must be copied
			/*
				TODO: Safety for associative containers (set,unordered_set).

				Copying script objects will call script functions and custom 
				script functions might not perfectly copy whatever is keeping
				the associative container sorted, thus sorting might explode.

				Implement some flag to tell the container that your custom
				script copy function is indeed safe. If the flag is set, use
				simple copy like this, otherwise copy all objects first and
				insert to the new container after copy.
			*/

			T_container::iterator it = T_container::begin();
			T_container::iterator it_other = const_cast<aatc_container_shared_1tp_template&>(other).begin();
			T_container::iterator it_end = T_container::end();

			for(; it != it_end;){
				void*& iii = const_cast<void*&>(*it);
				void*& iii_other = const_cast<void*&>(*it_other);

				iii = engine->CreateScriptObjectCopy(iii_other, objtype_content);

				it++;
				it_other++;
			}
		}
	}
	//aatc_container_shared_1tp_template(const aatc_container_shared_1tp_template& other){
	//	(*this) = other;
	//}

	void SetDirectcomp(bool ss){
		if(directcomp_forced){return;}
		handlemode_directcomp = ss;
	}

	//handle input is void* that points at handle which is a void*
	//handle output is void* that points at handle which is a void*

	void StoreHandle(void** target, void* ptr_to_handle){
		*target = *(void**)ptr_to_handle;
		if (handlemode_needref){
			engine->AddRefScriptObject(*target, objtype_content);
		}
	}
	void* StoreHandle2(void* ptr_to_handle){
		void* result = *(void**)ptr_to_handle;
		if(handlemode_needref){
			engine->AddRefScriptObject(result, objtype_content);
		}
		return result;
	}
	void ReleaseHandle(void* handle){
		if (handlemode_needref){
			engine->ReleaseScriptObject(handle, objtype_content);
		}
	}

	aatc_type_sizetype Count(void* value){
		if(handlemode_directcomp){
			return (aatc_type_sizetype)(std::count(T_container::begin(), T_container::end(), *(void**)value));
		}else{
			aatc_errorcheck_container_missingfunctions_operation_retnull(aatc_CONTAINER_OPERATION::COUNT, objtype_container->GetName(), objtype_content->GetName(), "count")

			if (handlemode){value = *(void**)value;}

			asIScriptContext* cc = els->contextcache_Get();

			aatc_type_sizetype count = 0;
			T_container::iterator it = T_container::begin();
			T_container::iterator itend = T_container::end();

			asIScriptFunction* func = func_cmp;
			if (func_equals){ func = func_equals; }

			if (func_equals){
				for (; it != itend; it++){
					cc->Prepare(func);
					cc->SetObject(value);
					cc->SetArgObject(0, *it);
					cc->Execute();
					count += (cc->GetReturnByte());
				}
			}else{//func cmp
				for (; it != itend; it++){
					cc->Prepare(func);
					cc->SetObject(value);
					cc->SetArgObject(0, *it);
					cc->Execute();
					count += (cc->GetReturnDWord() == 0);
				}
			}
			els->contextcache_Return(cc);

			return count;
		}
	};
	bool Contains_generic(void* value){ return Count(value) > 0; }


	void Clear(){
		T_container::iterator it = T_container::begin();
		T_container::iterator itend = T_container::end();
		if (handlemode){
			for (; it != itend; it++){
				ReleaseHandle(*it);
			}
		}else{
			for (; it != itend; it++){
				engine->ReleaseScriptObject(*it, objtype_content);
			}
		}
		T_container::clear();
	}
	bool Empty(){ return T_container::empty(); }
	aatc_type_sizetype Size(){ return (aatc_type_sizetype)(T_container::size()); }

	void EnumReferences(asIScriptEngine* engine){
		if (astypeid_content & asTYPEID_MASK_OBJECT){//dont do this for primitives
			T_container::iterator it = T_container::begin();
			T_container::iterator itend = T_container::end();
			for (; it != itend; it++){
				engine->GCEnumCallback(*it);
			}
		}
	}
	void ReleaseAllReferences(asIScriptEngine* engine){
		Clear();
	}

	//shared functions

	template<class T> void Push_Back(void* value){}
	template<class T> void Pop_Back(){}
	template<> void Push_Back<aatc_Y>(void* value){
		if (handlemode){
			push_back(NULL);
			StoreHandle(&back(), value);
		}
		else{
			push_back(engine->CreateScriptObjectCopy(value, objtype_content));
		}
	}
	template<> void Pop_Back<aatc_Y>(){
		if (handlemode){
			ReleaseHandle(back());
		}
		else{
			engine->ReleaseScriptObject(back(), objtype_content);
		}
		pop_back();
	}
	template<class T_cond> static void Register_func_back_write(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_back_write<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(const T&in)", aatc_name_script_container_method_push_back);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Push_Back<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s()", aatc_name_script_container_method_pop_back);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Pop_Back<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void* Back(){}
	template<> void* Back<aatc_Y>(){
		aatc_errorcheck_container_access_empty_retnull(objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_back)
		if (handlemode){
			return &(back());//return pointer to handle
		}else{
			return back();//return copy of pointer to object
		}
	}
	template<class T_cond> static void Register_func_back_read(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_back_read<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "T& %s()", aatc_name_script_container_method_back);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Back<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Push_Front(void* value){}
	template<class T> void Pop_Front(){}
	template<> void Push_Front<aatc_Y>(void* value){
		if (handlemode){
			push_front(NULL);
			StoreHandle(&front(), value);
		}else{
			push_front(engine->CreateScriptObjectCopy(value, objtype_content));
		}
	}
	template<> void Pop_Front<aatc_Y>(){
		if (handlemode){
			ReleaseHandle(front());
		}else{
			engine->ReleaseScriptObject(front(), objtype_content);
		}
		pop_front();
	}
	template<class T_cond> static void Register_func_front_write(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_front_write<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(const T&in)", aatc_name_script_container_method_push_front);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Push_Front<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s()", aatc_name_script_container_method_pop_front);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Pop_Front<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void* Front(){}
	template<> void* Front<aatc_Y>(){
		aatc_errorcheck_container_access_empty_retnull(objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_front)
		if (handlemode){
			return &(front());//return pointer to handle
		}else{
			return front();//return copy of pointer to object
		}
	}
	template<class T_cond> static void Register_func_front_read(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_front_read<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "T& %s()", aatc_name_script_container_method_front);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Front<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	//template<class T> void Erase_position(int position){}
	//template<> void Erase_position<aatc_Y>(int position){
	//	aatc_errorcheck_container_access_empty_retvoid(objtype_container->GetName(), objtype_content->GetName(), "erase_position")
	//	if ((position > -1) && (position < T_container::size())){
	//		T_container::iterator ii = T_container::begin() + position;

	//		if (handlemode){
	//			ReleaseHandle(*ii);
	//		}
	//		else{
	//			engine->ReleaseScriptObject(*ii, objtype_content);
	//		}

	//		T_container::erase(ii);
	//	}
	//}
	//template<class T_cond> static void Register_func_erase_position(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	//template<> static void Register_func_erase_position<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
	//	sprintf_s(textbuf,1000, "void erase(%s)", aatc_name_script_sizetype);
	//	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_position<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	//}

	template<class T> void Erase_value(void* value){}
	template<> void Erase_value<aatc_Y>(void* value){
		if (handlemode){
			if(!handlemode_directcomp){
				aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::ERASE_VALUE, objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_erase)
			}
			T_container::iterator it = T_container::find(*(void**)value);
			if (it != T_container::end()){
				ReleaseHandle(*it);
				T_container::erase(it);
			}
		} else{
			aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::ERASE_VALUE, objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_erase)

			T_container::iterator it = T_container::find(value);
			if (it != T_container::end()){
				void* object_to_release = *it;
				T_container::erase(it);//erasing needs object alive to call opCmp
				engine->ReleaseScriptObject(object_to_release, objtype_content);
			}
		}
	}
	template<class T_cond> static void Register_func_erase_value(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_erase_value<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(const T&in)", aatc_name_script_container_method_erase);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_value<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void* op_index(int index){}
	template<> void* op_index<aatc_Y>(int index){
		aatc_errorcheck_container_access_empty_retnull(objtype_container->GetName(), objtype_content->GetName(), "opIndex")
		if (handlemode){
			return &(T_container::operator[](index));//return pointer to handle
		}else{
			return T_container::operator[](index);//return pointer to object
		}
	}
	template<class T_cond> static void Register_func_op_index(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_op_index<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf,1000, "T& opIndex(%s)", aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, op_index<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Reserve(aatc_type_sizetype count){}
	template<> void Reserve<aatc_Y>(aatc_type_sizetype count){
		T_container::reserve(count);
	}
	template<class T_cond> static void Register_func_reserve(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_reserve<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(%s)", aatc_name_script_container_method_reserve, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Reserve<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Insert(void* value){}
	template<> void Insert<aatc_Y>(void* value){
		if (handlemode){
			if(!handlemode_directcomp){
				aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::INSERT, objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_insert)
			}
			std::pair<T_container::iterator, bool> rr = T_container::insert(*(void**)value);
			if (rr.second){
				StoreHandle((void**)&(*rr.first), value);
			}
		}else{
			aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::INSERT, objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_insert)

			T_container::iterator it = T_container::find(value);
			if (it == T_container::end()){
				T_container::insert(engine->CreateScriptObjectCopy(value, objtype_content));
			}
		}
	}
	template<class T_cond> static void Register_func_insert(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_insert<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(const T&in)", aatc_name_script_container_method_insert);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Insert<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}





//doxygen skip
#ifndef DOXYGEN_SHOULD_SKIP_THIS

	class scriptfunctor_cmp{
	public:
		asIScriptContext* cc;
		asIScriptFunction* func;

		bool operator()(void*& lhs, void*& rhs){
			cc->Prepare(func);
				cc->SetObject(lhs);
				cc->SetArgObject(0, rhs);
			cc->Execute();
			return cc->GetReturnDWord() == -1;
		}
	};
	class scriptfunctor_cmp_customscript{
	public:
		asIScriptContext* cc;
		aatc_script_Funcpointer* funcptr;

		bool operator()(void*& lhs, void*& rhs){
			funcptr->Prepare(cc);
			cc->SetArgObject(0, lhs);
			cc->SetArgObject(1, rhs);
			funcptr->Execute(cc);

			return cc->GetReturnDWord() == -1;
		}
	};


private:
	//didnt want to write the sort function twice so I went full meta... always go full meta
	template<typename T_is_native, typename T_functor> class _aatc_sort_basefunctor{};
	//native
	template<typename T_functor> class _aatc_sort_basefunctor<aatc_Y, T_functor>{
	public:
		void operator()(T_container& host, T_functor& functor){host.sort(functor);}
		void operator()(T_container& host){operator()(host, T_functor());}
	};
	//generic
	template<typename T_functor> class _aatc_sort_basefunctor<aatc_N, T_functor>{
	public:
		void operator()(T_container& host, T_functor& functor){std::sort(host.begin(), host.end(), functor);}
		void operator()(T_container& host){operator()(host, T_functor());}
	};

	//template<typename T_is_native> inline void _Sort_shared(const bool& ascending){
	//	if(handlemode_directcomp){
	//		if(ascending){
	//			_aatc_sort_basefunctor<T_is_native, std::less<void*>> a; a(*this);
	//		} else{
	//			_aatc_sort_basefunctor<T_is_native, aatc_functor_notcmp<void*, std::less<void*>>> a; a(*this);
	//		}
	//		return;
	//	}

	//	aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::SORT, objtype_container->GetName(), objtype_content->GetName(), "sort")
	//}
public:

#endif
//doxygen skip

	//template<typename T_included, typename T_is_native> void Sort(bool ascending){}
	template<typename T_is_native> void Sort(bool ascending){
		if(handlemode_directcomp){
			if(ascending){
				_aatc_sort_basefunctor<T_is_native, std::less<void*>> a; a(*this);
			} else{
				_aatc_sort_basefunctor<T_is_native, aatc_functor_notcmp<void*, std::less<void*>>> a; a(*this);
			}
			return;
		}

		aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::SORT, objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_sort)

		//_Sort_shared<T_is_native>(ascending);

		scriptfunctor_cmp functor;
			functor.cc = els->contextcache_Get();
			functor.func = func_cmp;

		if(ascending){
			_aatc_sort_basefunctor<T_is_native, scriptfunctor_cmp> a; a(*this, functor);
		} else{
			_aatc_sort_basefunctor<T_is_native, aatc_functor_notcmp_persistent_noconst<void*, scriptfunctor_cmp>> a; a(*this, aatc_functor_notcmp_persistent_noconst<void*, scriptfunctor_cmp>(functor));
		}

		els->contextcache_Return(functor.cc);
	}
	//template<typename T_included, typename T_is_native> void Sort_custom(aatc_script_Funcpointer* funcptr, bool ascending){}
	template<typename T_is_native> void Sort_custom(aatc_script_Funcpointer* funcptr, bool ascending){
		if(handlemode_directcomp){
			if(ascending){
				_aatc_sort_basefunctor<T_is_native, std::less<void*>> a; a(*this);
			} else{
				_aatc_sort_basefunctor<T_is_native, aatc_functor_notcmp<void*, std::less<void*>>> a; a(*this);
			}
			return;
		}

		aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::SORT, objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_sort)

		//_Sort_shared<T_is_native>(ascending);

		scriptfunctor_cmp_customscript functor;
			functor.cc = els->contextcache_Get();
			functor.funcptr = funcptr;

		if(ascending){
			_aatc_sort_basefunctor<T_is_native, scriptfunctor_cmp_customscript> a; a(*this, functor);
		} else{
			_aatc_sort_basefunctor<T_is_native, aatc_functor_notcmp_persistent_noconst<void*, scriptfunctor_cmp_customscript>> a; a(*this, aatc_functor_notcmp_persistent_noconst<void*, scriptfunctor_cmp_customscript>(functor));
		}

		els->contextcache_Return(functor.cc);
	}


	template<class T_cond> static void Register_func_sort_native(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_sort_native<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(bool ascending = true)", aatc_name_script_container_method_sort);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Sort<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(%s@,bool ascending = true)", aatc_name_script_container_method_sort, aatc_name_script_funcpointer);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Sort_custom<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T_cond> static void Register_func_sort_generic(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_sort_generic<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(bool ascending = true)", aatc_name_script_container_method_sort);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Sort<aatc_N>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(%s@,bool ascending = true)", aatc_name_script_container_method_sort, aatc_name_script_funcpointer);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Sort_custom<aatc_N>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> bool Contains_native(void* value){}
	template<> bool Contains_native<aatc_Y>(void* value){
		if(!handlemode_directcomp){
			aatc_errorcheck_container_missingfunctions_operation_retnull(aatc_CONTAINER_OPERATION::CONTAINS_NATIVE, objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_contains)
		}
		if(handlemode){ value = *(void**)value; }

		T_container::const_iterator it = T_container::find(value);
		return it != T_container::end();
	}
	template<class T_cond> static void Register_func_contains_native(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "bool %s(const T&in)", aatc_name_script_container_method_contains);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Contains_generic), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_contains_native<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "bool %s(const T&in)", aatc_name_script_container_method_contains);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Contains_native<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}





	//doxygen skip
	#ifndef DOXYGEN_SHOULD_SKIP_THIS
	private:
		template<typename T_is_native> class _functor_iterator_setindex{};
		//native (vector)
		template<> class _functor_iterator_setindex<aatc_Y>{
		public:
			void operator()(iteratortype& ii, std::size_t index){
				ii += index;
			}
		};
		//generic (list)
		template<> class _functor_iterator_setindex<aatc_N>{
		public:
			void operator()(iteratortype& ii, std::size_t index){
				for(int i = 0; i < index; i++){
					ii++;
				}
			}
		};
	public:
	#endif
	//doxygen skip

	template<class T_is_native> void Insert_generic_index_before(int index, void* value){
		if((index > -1) && (index <= T_container::size())){
			T_container::iterator ii = T_container::begin();
			{_functor_iterator_setindex<T_is_native> ff; ff(ii, index); }

			if(handlemode){
				insert(ii, StoreHandle2(value));
			} else{
				insert(ii,engine->CreateScriptObjectCopy(value, objtype_content));
			}
		}
	}

	template<class T_is_native> static void Register_func_insert_generic_index(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(%s,const T &in)", aatc_name_script_container_method_insert_index_before, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Insert_generic_index_before<T_is_native>), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_insert_generic_index<aatc_NULLCLASS>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}


	template<class T_is_native> void Erase_generic_index(int position){
		if((position > -1) && (position < T_container::size())){
			T_container::iterator ii = T_container::begin();

			{_functor_iterator_setindex<T_is_native> ff; ff(ii, position); }

			if(handlemode){
				ReleaseHandle(*ii);
			} else{
				engine->ReleaseScriptObject(*ii, objtype_content);
			}

			T_container::erase(ii);
		}
	}

	template<class T_is_native> void Erase_generic_index_range(int range_index_begin, int range_index_end){
		if((range_index_begin > -1) && (range_index_begin < T_container::size())){
			if((range_index_end > -1) && (range_index_end < T_container::size())){
				T_container::iterator ii_range_begin = T_container::begin();
				T_container::iterator ii_range_end = T_container::begin();

				{_functor_iterator_setindex<T_is_native> ff; ff(ii_range_begin, range_index_begin); }
				{_functor_iterator_setindex<T_is_native> ff; ff(ii_range_end, range_index_end); }

				{
					T_container::iterator iii = ii_range_begin;

					for(; iii != ii_range_end; iii++){
						engine->ReleaseScriptObject(*iii, objtype_content);
					}
				}
				
				T_container::erase(ii_range_begin, ii_range_end);
			}
		}
	}

	template<class T_is_native> static void Register_func_erase_generic_index(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(%s)", aatc_name_script_container_method_erase_index, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_generic_index<T_is_native>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(%s,%s)", aatc_name_script_container_method_erase_index, aatc_name_script_sizetype, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_generic_index_range<T_is_native>), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_erase_generic_index<aatc_NULLCLASS>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}



	template<class T_existence> void Erase_generic_value(void* value, bool all){
		if(handlemode){ value = *(void**)value; }

		if(handlemode_directcomp){
			bool continuu = 1;
			while(continuu){
				continuu = 0;
				bool found = 0;

				T_container::iterator it = T_container::begin();
				T_container::iterator itend = T_container::end();

				for(; it != itend; it++){
					if(*it == value){
						found = 1;
						engine->ReleaseScriptObject(*it, objtype_content);
						T_container::erase(it);
						break;
					}
				}
				if(empty()){ break; }

				if(found && all){ continuu = 1; }
			}
		} else{
			aatc_errorcheck_container_missingfunctions_operation_retvoid(aatc_CONTAINER_OPERATION::ERASE_GENERIC_VALUE, objtype_container->GetName(), objtype_content->GetName(), aatc_name_script_container_method_erase_value)

			asIScriptContext* cc = els->contextcache_Get();

			asIScriptFunction* func = func_cmp;
			if(func_equals){ func = func_equals; }

			bool continuu = 1;
			while(continuu){
				continuu = 0;
				bool found = 0;

				T_container::iterator it = T_container::begin();
				T_container::iterator itend = T_container::end();

				for(; it != itend; it++){
					cc->Prepare(func);
					cc->SetObject(value);
					cc->SetArgObject(0, *it);
					cc->Execute();

					if(func_equals){
						if(cc->GetReturnByte()){found = 1;}
					} else{
						if(cc->GetReturnDWord() == 0){ found = 1; }
					}
					if(found){
						found = 1;
						engine->ReleaseScriptObject(*it, objtype_content);
						T_container::erase(it);
						break;
					}
				}
				if(empty()){ break; }

				if(found && all){ continuu = 1; }
			}

			els->contextcache_Return(cc);
		}
	}
	template<class T_existence> void Erase_generic_value(void* value){Erase_generic_value<T_existence>(value, 0);}

	template<class T_existence> static void Register_func_erase_generic_value(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_erase_generic_value<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		//sprintf_s(textbuf, 1000, "void %s(const T &in value,bool all = false)", aatc_name_script_container_method_erase_value, aatc_name_script_sizetype);
		//r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_generic_value<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(const T &in value,bool all)", aatc_name_script_container_method_erase_value, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHODPR(aatc_container_shared_1tp_template, Erase_generic_value<aatc_Y>, (void*, bool), void), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(const T &in value)", aatc_name_script_container_method_erase_value, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHODPR(aatc_container_shared_1tp_template, Erase_generic_value<aatc_Y>, (void*), void), asCALL_THISCALL); assert(r >= 0);
	}
};


template<class T_container,
	bool cond_NEEDFUNC_BACK_WRITE = 0,
	bool cond_NEEDFUNC_BACK_READ = 0,
	bool cond_NEEDFUNC_FRONT_WRITE = 0,
	bool cond_NEEDFUNC_FRONT_READ = 0,
	bool cond_NEEDFUNC_ERASE_POSITION = 0,
	bool cond_NEEDFUNC_ERASE_VALUE = 0,
	bool cond_NEEDFUNC_OP_INDEX = 0,
	bool cond_NEEDFUNC_RESERVE = 0,
	bool cond_NEEDFUNC_INSERT = 0,
	bool cond_NEEDFUNC_SORT_NATIVE = 0,
	bool cond_NEEDFUNC_SORT_GENERIC = 0,
	bool cond_NEEDFUNC_CONTAINS_NATIVE = 0,
	typename cond_NEEDFUNC_ERASE_GENERIC_INDEX = aatc_NULLCLASS,
	bool cond_NEEDFUNC_ERASE_GENERIC_VALUE = 0,
	typename cond_NEEDFUNC_INSERT_GENERIC_INDEX = aatc_NULLCLASS
>
void aatc_container_shared_1tp_template_Register(asIScriptEngine* engine, const char* n_container){
	int r = 0;

	char n_container_T[1000];
	sprintf_s(n_container_T,1000, "%s<T>", n_container);

	char n_container_class_T[1000];
	sprintf_s(n_container_class_T, 1000, "%s<class T>", n_container);

	char textbuf[1000];

	r = engine->RegisterObjectType(n_container_class_T, 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s@ f(int&in)", n_container_T);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_FACTORY, textbuf, asFUNCTIONPR(T_container::Factory, (asIObjectType*), T_container*), asCALL_CDECL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s@ f(int&in,const %s &in)", n_container_T, n_container_T);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_FACTORY, textbuf, asFUNCTIONPR(T_container::Factory_copy, (asIObjectType*,const T_container&), T_container*), asCALL_CDECL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s& opAssign(const %s &in)", n_container_T, n_container_T);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, operator=), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s& %s(%s &in)", n_container_T, aatc_name_script_container_method_swap, n_container_T);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Swap), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_ADDREF, "void f()", asMETHOD(T_container, refcount_Add), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_RELEASE, "void f()", asMETHOD(T_container, refcount_Release), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_SETGCFLAG, "void f()", asMETHOD(aatc_refcounted_GC, SetGCFlag), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(aatc_refcounted_GC, GetGCFlag), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(aatc_refcounted_GC, GetRefCount), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_ENUMREFS, "void f(int&in)", asMETHODPR(T_container, EnumReferences, (asIScriptEngine*), void), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHODPR(T_container, ReleaseAllReferences, (asIScriptEngine*), void), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(aatc_templatecallback_container), asCALL_CDECL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "void %s(bool)", aatc_name_script_container_method_set_directcomp);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, SetDirectcomp), asCALL_THISCALL); assert(r >= 0);
	

	sprintf_s(textbuf, 1000, "%s %s(const T &in)", aatc_name_script_sizetype, aatc_name_script_container_method_count);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Count), asCALL_THISCALL); assert(r >= 0);
	//r = engine->RegisterObjectMethod(n_container_T, "bool contains(const T &in)", asMETHOD(T_container, Contains), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "void %s()", aatc_name_script_container_method_clear);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Clear), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "bool %s()", aatc_name_script_container_method_empty);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Empty), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s %s()", aatc_name_script_sizetype, aatc_name_script_container_method_size);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Size), asCALL_THISCALL); assert(r >= 0);

	T_container::Register_func_back_write<std::conditional<cond_NEEDFUNC_BACK_WRITE, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_back_read<std::conditional<cond_NEEDFUNC_BACK_READ, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_front_write<std::conditional<cond_NEEDFUNC_FRONT_WRITE, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_front_read<std::conditional<cond_NEEDFUNC_FRONT_READ, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	//T_container::Register_func_erase_position<std::conditional<cond_NEEDFUNC_ERASE_POSITION, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_erase_value<std::conditional<cond_NEEDFUNC_ERASE_VALUE, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_op_index<std::conditional<cond_NEEDFUNC_OP_INDEX, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_reserve<std::conditional<cond_NEEDFUNC_RESERVE, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_insert<std::conditional<cond_NEEDFUNC_INSERT,aatc_Y,aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);

	T_container::Register_func_sort_native<std::conditional<cond_NEEDFUNC_SORT_NATIVE, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_sort_generic<std::conditional<cond_NEEDFUNC_SORT_GENERIC, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);

	T_container::Register_func_contains_native<std::conditional<cond_NEEDFUNC_CONTAINS_NATIVE, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);

	T_container::Register_func_erase_generic_index<cond_NEEDFUNC_ERASE_GENERIC_INDEX>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_erase_generic_value<std::conditional<cond_NEEDFUNC_ERASE_GENERIC_VALUE, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);

	T_container::Register_func_insert_generic_index<cond_NEEDFUNC_INSERT_GENERIC_INDEX>(engine, r, textbuf, n_container, n_container_T);
}

template<typename T_out, typename T_host> T_out aatc_reghelp_construct_hosted_iterator_template(T_host cont){
	//return T_out(&cont, 0, nullptr);
	return T_out(&cont, 0);
}


/*!\brief Internal template monster

*/
template<class T_container> class aect_iterator_shared_template{
public:
	uint8_t testbytess[100];

	T_container* host;

	//#if aatc_container_shared_1tp_USEFUNCS_CONST_ITERATOR_ONLY
	//	typename T_container::const_iterator it;
	//	typename T_container::const_iterator it_end;
	//#else
	//	typename T_container::iterator it;
	//	typename T_container::iterator it_end;
	//#endif

	typename T_container::iterator it;
	typename T_container::iterator it_end;

	bool cont;
	bool firstt;
	bool handlemode;

	int tester;

	aect_iterator_shared_template() :
		it(),
		it_end(),
		tester(88)
	{
		for(int i = 0; i < 100; i++){
			testbytess[i] = i;
		}
	}
	aect_iterator_shared_template(void *ref, int typeId_target_container):
		firstt(1),
		it(),
		it_end(),
		tester(22)
	{
		for(int i = 0; i < 100; i++){
			testbytess[i] = i;
		}
		host = (T_container*)(*(void**)ref);
		Init();
	}
	aect_iterator_shared_template(const aect_iterator_shared_template& other) :
		host(other.host),
		it(other.it),
		it_end(other.it_end),
		firstt(other.firstt),
		cont(other.cont),
		handlemode(other.handlemode)
	{}

	aect_iterator_shared_template& operator=(const aect_iterator_shared_template& other){
		host = other.host;
		it = other.it;
		it_end = other.it_end;
		firstt = other.firstt;
		cont = other.cont;
		handlemode = other.handlemode;

		return *this;
	}

	void Init(){
		if (host->empty()){
			cont = 0;
		}else{
			handlemode = host->handlemode;

			it = host->begin();
			it_end = host->end();
			cont = 1;
		}
	}

	//combine end check and continuation into one monster
	bool Next(){
		if (firstt){
			if (cont){//all is well
				firstt = 0;
				return 1;
			}else{//cont set to 0 in constructor because container is empty
				return 0;
			}
		}else{
			it++;
			//if (it == host->end()){
			if (it == it_end){
				return 0;
			}else{
				return 1;
			}
		}
	}

	template<class T> const void* Current_get(){}
	template<class T> void Current_set(void* newval){}
	template<> const void* Current_get<aatc_Y>(){
		if(handlemode){
			return &(*it);//return pointer to handle
		} else{
			return *it;//return copy of pointer to object
		}
	}
	template<> void Current_set<aatc_Y>(void* value){
		if(handlemode){
			if(*it){
				host->engine->ReleaseScriptObject(*it, host->objtype_content);
			}
			if(value){
				*it = host->StoreHandle2(value);
			} else{
				*it = nullptr;
			}
		} else{
			host->engine->ReleaseScriptObject(*it, host->objtype_content);
			*it = host->engine->CreateScriptObjectCopy(value, host->objtype_content);
		}
	}

	template<class T> void* Current(){}
	template<> void* Current<aatc_Y>(){
		if (handlemode){
			return &(*(it));//return pointer to handle
		}else{
			return *(it);//return copy of pointer to object
		}
	}
	template<class T_cond> static void Register_func_current(asIScriptEngine* engine, int& r, const char* n_iterator_T){}
	template<> static void Register_func_current<aatc_Y>(asIScriptEngine* engine, int& r, const char* n_iterator_T){
		char textbuf[1000];
		sprintf_s(textbuf, 1000, "T& %s()", aatc_name_script_iterator_access_function);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_template, Current<aatc_Y>), asCALL_THISCALL); assert(r >= 0);

		sprintf_s(textbuf, 1000, "T& get_%s()", aatc_name_script_iterator_access_property);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_template, Current_get<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void set_%s(const T &in)", aatc_name_script_iterator_access_property);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_template, Current_set<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> const void* Current_const(){}
	template<> const void* Current_const<aatc_Y>(){
		if (handlemode){
			return &(*(it));//return pointer to handle
		}else{
			return *(it);//return copy of pointer to object
		}
	}
	template<class T_cond> static void Register_func_current_const(asIScriptEngine* engine, int& r, const char* n_iterator_T){}
	template<> static void Register_func_current_const<aatc_Y>(asIScriptEngine* engine, int& r, const char* n_iterator_T){
		char textbuf[1000];
		sprintf_s(textbuf, 1000, "const T& %s()", aatc_name_script_iterator_access_function);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_template, Current_const<aatc_Y>), asCALL_THISCALL); assert(r >= 0);

		sprintf_s(textbuf, 1000, "const T& get_%s()", aatc_name_script_iterator_access_property);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_template, Current_get<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}


	static void static_constructor_default(asIObjectType* objtype, void *memory){
		new(memory)aect_iterator_shared_template();
	}
	static void static_constructor(asIObjectType* objtype, void *ref, int typeId, void *memory){
		new(memory)aect_iterator_shared_template(ref, typeId);
	}
	static void static_constructor_copy(asIObjectType* objtype, const aect_iterator_shared_template& other, void *memory){
		new(memory)aect_iterator_shared_template(other);
	}

	template<class cond_EDITABLE = aatc_Y, class cond_CONST = aatc_N> static void Register(asIScriptEngine* engine, const char* n_iterator, const char* n_container_T){
		int r = 0;
		char textbuf[1000];

		char n_iterator_T[1000];
		sprintf_s(n_iterator_T, 1000, "%s<T>", n_iterator);

		char n_iterator_class_T[1000];
		sprintf_s(n_iterator_class_T, 1000, "%s<class T>", n_iterator);

		r = engine->RegisterObjectType(n_iterator_class_T, sizeof(aect_iterator_shared_template), asOBJ_VALUE | asOBJ_TEMPLATE | asGetTypeTraits<aect_iterator_shared_template>()); assert(r >= 0);

		//the default constructor must be registered, but you should never use it in script
		//r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in)", asFunctionPtr(aatc_reghelp_constructor_template_default<aect_iterator_shared_template>), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in)", asFunctionPtr(static_constructor_default), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in,?&in)", asFunctionPtr(static_constructor), asCALL_CDECL_OBJLAST); assert(r >= 0);

		//sprintf_s(textbuf, 1000, "void f(int&in,const %s &in)", n_iterator_T);
		//r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(static_constructor_copy), asCALL_CDECL_OBJLAST); assert(r >= 0);

		//sprintf(textbuf, "void f(int&in,%s@)", n_container_T);
		//r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(static_constructor), asCALL_CDECL_OBJLAST); assert(r >= 0);
		
		//sprintf_s(textbuf, 1000, "void f(const %s &in)", n_iterator_T);
		//r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(aatc_reghelp_constructor_copy<aect_iterator_shared_template, aect_iterator_shared_template>), asCALL_CDECL_OBJLAST); assert(r >= 0);

		r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(aatc_reghelp_generic_destructor<aect_iterator_shared_template>), asCALL_CDECL_OBJLAST); assert(r >= 0);

		aect_iterator_shared_template::Register_func_current<cond_EDITABLE>(engine, r, n_iterator_T);
		aect_iterator_shared_template::Register_func_current_const<cond_CONST>(engine, r, n_iterator_T);

		r = engine->RegisterObjectMethod(n_iterator_T, "bool next()", asMETHOD(aect_iterator_shared_template, Next), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(n_iterator_T, "bool opPreInc()", asMETHOD(aect_iterator_shared_template, Next), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(n_iterator_T, "bool opPostInc()", asMETHOD(aect_iterator_shared_template, Next), asCALL_THISCALL); assert(r >= 0);

		//sprintf_s(textbuf, 1000, "void not_opAssign(const %s &in)", n_iterator_T, n_iterator_T);
		//sprintf_s(textbuf, 1000, "void opAssign(const %s &in)", n_iterator_T, n_iterator_T);
		//r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_template, not_op_assign), asCALL_THISCALL); assert(r >= 0);

		sprintf_s(textbuf, 1000, "%s& opAssign(const %s &in)", n_iterator_T, n_iterator_T);
		r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aect_iterator_shared_template, operator=), asCALL_THISCALL); assert(r >= 0);
	}
};



END_AS_NAMESPACE

#endif