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
template<class T_container, int T_CONTAINERTYPEID, class bcw = aatc_bcw_basic<T_container>> class aatc_container_shared_1tp_template :	public aatc_container_base,
																																		public bcw,
																																		public aatc_refcounted_GC,
																																		public aatc_containerfunctor_Settings
{
public:
	typename typedef T_container::iterator iteratortype;

	//typename typedef std::conditional<NEED_COMPHELPER, aatc_bcw_comphelper<T_container>, aatc_bcw_basic<T_container>>::type bct;
	//typename typedef bcw bct;

	asIObjectType* objtype_container;
	asIObjectType* objtype_content;
	config::t::astypeid astypeid_container;
	config::t::astypeid astypeid_content;
	asDWORD typeflags_container;
	asDWORD typeflags_content;

	bool handlemode_needref;

	engine_level_storage* els;

	aatc_container_operations_bitmask_type missing_functions;

	bool directcomp_forced;

	aatc_container_shared_1tp_template(asIScriptEngine* _engine, asIObjectType* _objtype) :
		bcw(_engine, this),
		aatc_refcounted_GC(),
		objtype_container(_objtype),
		directcomp_forced(0)
	{
		engine = _engine;

		handlemode_directcomp = config::DEFAULT_HANDLEMODE_DIRECTCOMP;

		objtype_content = objtype_container->GetSubType();

		astypeid_container = objtype_container->GetTypeId();
		astypeid_content = objtype_container->GetSubTypeId();

		typeflags_container = objtype_container->GetFlags();
		typeflags_content = objtype_content->GetFlags();

		//problem_nofunc_eq_or_cmp = 0;
		//problem_nofunc_cmp = 0;

		els = aatc_Get_ELS(engine);
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
	}
	void operator=(const aatc_container_shared_1tp_template& other){
		aatc_errorcheck_container_iterator_safety_version_Increment();

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
	void swap(aatc_container_shared_1tp_template& other){
		T_container::swap(static_cast<T_container&>(other));
		aatc_errorcheck_container_iterator_safety_version_Increment();
		#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
			other.iterator_safety_version++;
		#endif
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
		//if (handlemode_needref){
			engine->AddRefScriptObject(*target, objtype_content);
		//}
	}
	void* StoreHandle2(void* ptr_to_handle){
		void* result = *(void**)ptr_to_handle;
		//if(handlemode_needref){
			engine->AddRefScriptObject(result, objtype_content);
		//}
		return result;
	}
	void ReleaseHandle(void* handle){
		//if (handlemode_needref){
			engine->ReleaseScriptObject(handle, objtype_content);
		//}
	}

	config::t::sizetype Count(void* value){
		if(handlemode_directcomp){
			return (config::t::sizetype)(std::count(T_container::begin(), T_container::end(), *(void**)value));
		}else{
			aatc_errorcheck_container_missingfunctions_operation_retnull(CONTAINER_OPERATION::COUNT, objtype_container->GetName(), objtype_content->GetName(), "count")

			if (handlemode){value = *(void**)value;}

			asIScriptContext* cc = els->contextcache_Get();

			config::t::sizetype count = 0;
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
		aatc_errorcheck_container_iterator_safety_version_Increment();

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
	config::t::sizetype Size(){ return (config::t::sizetype)(T_container::size()); }

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
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if (handlemode){
			push_back(NULL);
			StoreHandle(&back(), value);
		}
		else{
			push_back(engine->CreateScriptObjectCopy(value, objtype_content));
		}
	}
	template<> void Pop_Back<aatc_Y>(){
		aatc_errorcheck_container_iterator_safety_version_Increment();

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
		sprintf_s(textbuf, 1000, "void %s(const T&in)", config::scriptname::method::container::push_back);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Push_Back<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s()", config::scriptname::method::container::pop_back);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Pop_Back<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void* Back(){}
	template<> void* Back<aatc_Y>(){
		aatc_errorcheck_container_access_empty_retnull(objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::back)
		if (handlemode){
			return &(back());//return pointer to handle
		}else{
			return back();//return copy of pointer to object
		}
	}
	template<class T_cond> static void Register_func_back_read(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_back_read<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "T& %s()", config::scriptname::method::container::back);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Back<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Push_Front(void* value){}
	template<class T> void Pop_Front(){}
	template<> void Push_Front<aatc_Y>(void* value){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if (handlemode){
			push_front(NULL);
			StoreHandle(&front(), value);
		}else{
			push_front(engine->CreateScriptObjectCopy(value, objtype_content));
		}
	}
	template<> void Pop_Front<aatc_Y>(){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if (handlemode){
			ReleaseHandle(front());
		}else{
			engine->ReleaseScriptObject(front(), objtype_content);
		}
		pop_front();
	}
	template<class T_cond> static void Register_func_front_write(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_front_write<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(const T&in)", config::scriptname::method::container::push_front);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Push_Front<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s()", config::scriptname::method::container::pop_front);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Pop_Front<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void* Front(){}
	template<> void* Front<aatc_Y>(){
		aatc_errorcheck_container_access_empty_retnull(objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::front)
		if (handlemode){
			return &(front());//return pointer to handle
		}else{
			return front();//return copy of pointer to object
		}
	}
	template<class T_cond> static void Register_func_front_read(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_front_read<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "T& %s()", config::scriptname::method::container::front);
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
	//	sprintf_s(textbuf,1000, "void erase(%s)", config::scriptname::t::size);
	//	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_position<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	//}

	template<class T> void Erase_value(void* value){}
	template<> void Erase_value<aatc_Y>(void* value){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if (handlemode){
			if(!handlemode_directcomp){
				aatc_errorcheck_container_missingfunctions_operation_retvoid(CONTAINER_OPERATION::ERASE_VALUE, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::erase)
			}
			T_container::iterator it = T_container::find(*(void**)value);
			if (it != T_container::end()){
				ReleaseHandle(*it);
				T_container::erase(it);
			}
		} else{
			aatc_errorcheck_container_missingfunctions_operation_retvoid(CONTAINER_OPERATION::ERASE_VALUE, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::erase)

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
		sprintf_s(textbuf, 1000, "void %s(const T&in)", config::scriptname::method::container::erase);
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
		sprintf_s(textbuf,1000, "T& opIndex(%s)", config::scriptname::t::size);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, op_index<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Reserve(config::t::sizetype count){}
	template<> void Reserve<aatc_Y>(config::t::sizetype count){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		T_container::reserve(count);
	}
	template<class T_cond> static void Register_func_reserve(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_reserve<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(%s)", config::scriptname::method::container::reserve, config::scriptname::t::size);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Reserve<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Insert(void* value){}
	template<> void Insert<aatc_Y>(void* value){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if (handlemode){
			if(!handlemode_directcomp){
				aatc_errorcheck_container_missingfunctions_operation_retvoid(CONTAINER_OPERATION::INSERT, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::insert)
			}
			std::pair<T_container::iterator, bool> rr = T_container::insert(*(void**)value);
			if (rr.second){
				StoreHandle((void**)&(*rr.first), value);
			}
		}else{
			aatc_errorcheck_container_missingfunctions_operation_retvoid(CONTAINER_OPERATION::INSERT, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::insert)

			T_container::iterator it = T_container::find(value);
			if (it == T_container::end()){
				T_container::insert(engine->CreateScriptObjectCopy(value, objtype_content));
			}
		}
	}
	template<class T_cond> static void Register_func_insert(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_insert<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(const T&in)", config::scriptname::method::container::insert);
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

	//	aatc_errorcheck_container_missingfunctions_operation_retvoid(CONTAINER_OPERATION::SORT, objtype_container->GetName(), objtype_content->GetName(), "sort")
	//}
public:

#endif
//doxygen skip

	//template<typename T_included, typename T_is_native> void Sort(bool ascending){}
	template<typename T_is_native> void Sort(bool ascending){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if(handlemode_directcomp){
			if(ascending){
				_aatc_sort_basefunctor<T_is_native, std::less<void*>> a; a(*this);
			} else{
				_aatc_sort_basefunctor<T_is_native, aatc_functor_notcmp<void*, std::less<void*>>> a; a(*this);
			}
			return;
		}

		aatc_errorcheck_container_missingfunctions_operation_retvoid(CONTAINER_OPERATION::SORT, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::sort)

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
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if(handlemode_directcomp){
			if(ascending){
				_aatc_sort_basefunctor<T_is_native, std::less<void*>> a; a(*this);
			} else{
				_aatc_sort_basefunctor<T_is_native, aatc_functor_notcmp<void*, std::less<void*>>> a; a(*this);
			}
			return;
		}

		aatc_errorcheck_container_missingfunctions_operation_retvoid(CONTAINER_OPERATION::SORT, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::sort)

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
		sprintf_s(textbuf, 1000, "void %s(bool ascending = true)", config::scriptname::method::container::sort);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Sort<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(%s@,bool ascending = true)", config::scriptname::method::container::sort, aatc_name_script_funcpointer);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Sort_custom<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T_cond> static void Register_func_sort_generic(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}
	template<> static void Register_func_sort_generic<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "void %s(bool ascending = true)", config::scriptname::method::container::sort);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Sort<aatc_N>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(%s@,bool ascending = true)", config::scriptname::method::container::sort, aatc_name_script_funcpointer);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Sort_custom<aatc_N>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> bool Contains_native(void* value){}
	template<> bool Contains_native<aatc_Y>(void* value){
		if(!handlemode_directcomp){
			aatc_errorcheck_container_missingfunctions_operation_retnull(CONTAINER_OPERATION::CONTAINS_NATIVE, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::contains)
		}
		if(handlemode){ value = *(void**)value; }

		T_container::const_iterator it = T_container::find(value);
		return it != T_container::end();
	}
	template<class T_cond> static void Register_func_contains_native(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "bool %s(const T&in)", config::scriptname::method::container::contains);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Contains_generic), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_contains_native<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){
		sprintf_s(textbuf, 1000, "bool %s(const T&in)", config::scriptname::method::container::contains);
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
		aatc_errorcheck_container_iterator_safety_version_Increment();

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
		sprintf_s(textbuf, 1000, "void %s(%s,const T &in)", config::scriptname::method::container::insert_index_before, config::scriptname::t::size);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Insert_generic_index_before<T_is_native>), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_insert_generic_index<aatc_NULLCLASS>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}


	template<class T_is_native> void Erase_generic_index(int position){
		aatc_errorcheck_container_iterator_safety_version_Increment();

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
		aatc_errorcheck_container_iterator_safety_version_Increment();

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
		sprintf_s(textbuf, 1000, "void %s(%s)", config::scriptname::method::container::erase_index, config::scriptname::t::size);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_generic_index<T_is_native>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(%s,%s)", config::scriptname::method::container::erase_index, config::scriptname::t::size, config::scriptname::t::size);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_generic_index_range<T_is_native>), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_erase_generic_index<aatc_NULLCLASS>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T){}



	template<class T_existence> void Erase_generic_value(void* value, bool all){
		aatc_errorcheck_container_iterator_safety_version_Increment();

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
			aatc_errorcheck_container_missingfunctions_operation_retvoid(CONTAINER_OPERATION::ERASE_GENERIC_VALUE, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::erase_value)

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
		//sprintf_s(textbuf, 1000, "void %s(const T &in value,bool all = false)", config::scriptname::method::container::erase_value, config::scriptname::t::size);
		//r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_generic_value<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(const T &in value,bool all)", config::scriptname::method::container::erase_value);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHODPR(aatc_container_shared_1tp_template, Erase_generic_value<aatc_Y>, (void*, bool), void), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(const T &in value)", config::scriptname::method::container::erase_value);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHODPR(aatc_container_shared_1tp_template, Erase_generic_value<aatc_Y>, (void*), void), asCALL_THISCALL); assert(r >= 0);
	}







	class aatc_iterator : public aatc_iterator_base{
	public:
		aatc_container_shared_1tp_template* host;

		typename aatc_container_shared_1tp_template::iterator it;
		typename aatc_container_shared_1tp_template::iterator it_end;

		bool handlemode;

		aatc_iterator() :
			it(),
			it_end()
		{}
		aatc_iterator(void *ref, config::t::astypeid typeId_targeaatc_container_shared_1tp_template) :
			it(),
			it_end()
		{
			host = (aatc_container_shared_1tp_template*)(*(void**)ref);
			Init();
		}
		aatc_iterator(const aatc_iterator& other) :
			aatc_iterator_base(other),

			host(other.host),
			it(other.it),
			it_end(other.it_end),

			handlemode(other.handlemode)
		{
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				iterator_safety_version = other.iterator_safety_version;
			#endif
		}
		~aatc_iterator(){}

		aatc_iterator& operator=(const aatc_iterator& other){
			host = other.host;
			it = other.it;
			it_end = other.it_end;

			firstt = other.firstt;
			cont = other.cont;

			handlemode = other.handlemode;

			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				iterator_safety_version = other.iterator_safety_version;
			#endif

			return *this;
		}

		void Init(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				iterator_safety_version = host->iterator_safety_version;
			#endif

			if(host->empty()){
				cont = 0;

				it = host->end();
				it_end = host->end();
			} else{
				handlemode = host->handlemode;

				it = host->begin();
				it_end = host->end();
				cont = 1;
			}
		}

		//combine end check and continuation into one monster
		bool Next(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				if(iterator_safety_version != host->iterator_safety_version){
					aatc_errorprint_iterator_container_modified();
					return 0;
				}
			#endif

			if(firstt){
				if(cont){//all is well
					firstt = 0;
					return 1;
				} else{//cont set to 0 in constructor because container is empty
					return 0;
				}
			} else{
				it++;
				//if (it == host->end()){
				if(it == it_end){
					return 0;
				} else{
					return 1;
				}
			}
		}

		template<class T> const void* Current_get(){}
		template<class T> void Current_set(void* newval){}
		template<> const void* Current_get<aatc_Y>(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				if(iterator_safety_version != host->iterator_safety_version){
					aatc_errorprint_iterator_container_modified();
					return nullptr;
				}
			#endif

			if(handlemode){
				return &(*it);//return pointer to handle
			} else{
				return *it;//return copy of pointer to object
			}
		}
		template<> void Current_set<aatc_Y>(void* value){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				if(iterator_safety_version != host->iterator_safety_version){
					aatc_errorprint_iterator_container_modified();
					return;
				}
			#endif

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
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				if(iterator_safety_version != host->iterator_safety_version){
					aatc_errorprint_iterator_container_modified();
					return nullptr;
				}
			#endif

			if(handlemode){
				return &(*(it));//return pointer to handle
			} else{
				return *(it);//return copy of pointer to object
			}
		}
		template<class T_cond> static void Register_func_current(asIScriptEngine* engine, int& r, const char* n_iterator_T){}
		template<> static void Register_func_current<aatc_Y>(asIScriptEngine* engine, int& r, const char* n_iterator_T){
			char textbuf[1000];
			sprintf_s(textbuf, 1000, "T& %s()", config::scriptname::method::iterator::access_function);
			r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aatc_iterator, Current<aatc_Y>), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "T& get_%s()", config::scriptname::method::iterator::access_property);
			r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aatc_iterator, Current_get<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
			sprintf_s(textbuf, 1000, "void set_%s(const T &in)", config::scriptname::method::iterator::access_property);
			r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aatc_iterator, Current_set<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		}

		template<class T> const void* Current_const(){}
		template<> const void* Current_const<aatc_Y>(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				if(iterator_safety_version != host->iterator_safety_version){
					aatc_errorprint_iterator_container_modified();
					return nullptr;
				}
			#endif

			if(handlemode){
				return &(*(it));//return pointer to handle
			} else{
				return *(it);//return copy of pointer to object
			}
		}
		template<class T_cond> static void Register_func_current_const(asIScriptEngine* engine, int& r, const char* n_iterator_T){}
		template<> static void Register_func_current_const<aatc_Y>(asIScriptEngine* engine, int& r, const char* n_iterator_T){
			char textbuf[1000];
			sprintf_s(textbuf, 1000, "const T& %s()", config::scriptname::method::iterator::access_function);
			r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aatc_iterator, Current_const<aatc_Y>), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "const T& get_%s()", config::scriptname::method::iterator::access_property);
			r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aatc_iterator, Current_get<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		}


		static void static_constructor_default(asIObjectType* objtype, void *memory){
			new(memory)aatc_iterator();
		}
		static void static_constructor_copy(asIObjectType* objtype, aatc_iterator* other, void *memory) {
			new(memory)aatc_iterator(*other);
		}
		static void static_constructor_parentcontainer(asIObjectType* objtype, void *ref, config::t::astypeid typeId, void *memory){
			new(memory)aatc_iterator(ref, typeId);
		}
		//static void static_constructor_copy(asIObjectType* objtype, const aatc_iterator& other, void *memory){
		//	new(memory)aatc_iterator(other);
		//}

		/*
		Using this in script should be faster than (it == container.end()) because container.end() creates an object
		*/
		bool IsEnd(){
			return it == it_end;
		}
		void SetToEnd(){
			firstt = 0;
			cont = 0;
			it = it_end;
		}
		bool IsValid(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				return iterator_safety_version == host->iterator_safety_version;
			#else
				return 1;
			#endif
		}

		bool operator==(const aatc_iterator& other){
			return it == other.it;
		}

		template<class cond_EDITABLE = aatc_Y, class cond_CONST = aatc_N> static void Register(asIScriptEngine* engine, const char* n_iterator, const char* n_container_T){
			int r = 0;
			char textbuf[1000];

			char n_iterator_T[1000];
			sprintf_s(n_iterator_T, 1000, "%s<T>", n_iterator);

			char n_iterator_class_T[1000];
			sprintf_s(n_iterator_class_T, 1000, "%s<class T>", n_iterator);

			r = engine->RegisterObjectType(n_iterator_class_T, sizeof(aatc_iterator), asOBJ_VALUE | asOBJ_TEMPLATE | asGetTypeTraits<aatc_iterator>()); assert(r >= 0);

			//the default constructor must be registered, but you should never use it in script
			//r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in)", asFunctionPtr(aatc_reghelp_constructor_template_default<aatc_iterator>), asCALL_CDECL_OBJLAST); assert(r >= 0);

			r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in)", asFunctionPtr(static_constructor_default), asCALL_CDECL_OBJLAST); assert(r >= 0);
			sprintf_s(textbuf, 1000, "void f(int&in,const %s &in)", n_iterator_T);
			r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(static_constructor_copy), asCALL_CDECL_OBJLAST); assert(r >= 0);
			r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_CONSTRUCT, "void f(int&in,?&in)", asFunctionPtr(static_constructor_parentcontainer), asCALL_CDECL_OBJLAST); assert(r >= 0);

			r = engine->RegisterObjectBehaviour(n_iterator_T, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(aatc_reghelp_generic_destructor<aatc_iterator>), asCALL_CDECL_OBJLAST); assert(r >= 0);

			aatc_iterator::Register_func_current<cond_EDITABLE>(engine, r, n_iterator_T);
			aatc_iterator::Register_func_current_const<cond_CONST>(engine, r, n_iterator_T);

			r = engine->RegisterObjectMethod(n_iterator_T, "bool next()", asMETHOD(aatc_iterator, Next), asCALL_THISCALL); assert(r >= 0);
			r = engine->RegisterObjectMethod(n_iterator_T, "bool opPreInc()", asMETHOD(aatc_iterator, Next), asCALL_THISCALL); assert(r >= 0);
			r = engine->RegisterObjectMethod(n_iterator_T, "bool opPostInc()", asMETHOD(aatc_iterator, Next), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "%s& opAssign(const %s &in)", n_iterator_T, n_iterator_T);
			r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aatc_iterator, operator=), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "bool opEquals(const %s &in)", n_iterator_T);
			r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aatc_iterator, operator==), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "bool %s()", config::scriptname::method::iterator::is_end);
			r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aatc_iterator, IsEnd), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "bool %s()", config::scriptname::method::iterator::is_valid);
			r = engine->RegisterObjectMethod(n_iterator_T, textbuf, asMETHOD(aatc_iterator, IsValid), asCALL_THISCALL); assert(r >= 0);
		}
	};



	aatc_iterator Begin(){
		void* vthis = this;
		return aatc_iterator(&vthis, 0);
	}
	aatc_iterator End(){
		void* vthis = this;
		aatc_iterator result(&vthis, 0);
		result.SetToEnd();
		return result;
	}



	template<class T> bool Erase_iterator(const aatc_iterator& aatc_it){}
	template<> bool Erase_iterator<aatc_Y>(const aatc_iterator& aatc_it){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
			if(iterator_safety_version != aatc_it.iterator_safety_version){
				common::errorprint::container::iterator_invalid();
				return 0;
			}
		#endif

		aatc_errorcheck_container_iterator_safety_version_Increment();

		T_container::iterator it = aatc_it.it;

		if(it == T_container::end()){
			return 0;
		} else{
			if(handlemode){
				ReleaseHandle(*it);
				T_container::erase(it);
			} else{
				void* object_to_release = *it;
				T_container::erase(it);//erasing needs object alive to call opCmp
				engine->ReleaseScriptObject(object_to_release, objtype_content);
			}

			return 1;
		}
	}
	template<class T_cond> static void Register_func_Erase_iterator(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_iterator_TT){}
	template<> static void Register_func_Erase_iterator<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "bool %s(const %s &in)", config::scriptname::method::container::erase_iterator, n_iterator_TT);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_iterator<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> config::t::sizetype Erase_range_iterator(const aatc_iterator& aatc_it_range_begin, const aatc_iterator& aatc_it_range_end){}
	template<> config::t::sizetype Erase_range_iterator<aatc_Y>(const aatc_iterator& aatc_it_range_begin, const aatc_iterator& aatc_it_range_end){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
			if((iterator_safety_version != aatc_it_range_begin.iterator_safety_version) || (iterator_safety_version != aatc_it_range_end.iterator_safety_version)){
				common::errorprint::container::iterator_invalid();
				return 0;
			}
		#endif

		aatc_errorcheck_container_iterator_safety_version_Increment();

		T_container::iterator it_range_begin = aatc_it_range_begin.it;
		T_container::iterator it_range_end = aatc_it_range_end.it;

		//if((it_range_begin == T_container::end()) && (it_range_end == T_container::end())){
		if(it_range_begin == it_range_end){
			return 0;
		} else{
			config::t::sizetype delcount = 0;

			if(handlemode){
				for(auto it = it_range_begin; it != it_range_end; it++){
					ReleaseHandle(*it);
					delcount++;
				}
				T_container::erase(it_range_begin, it_range_end);
			} else{
				std::vector<void*> objects_to_release;//ohhh my performance
				//objects_to_release.insert(it_range_begin, it_range_end);
				objects_to_release.reserve(T_container::size());
				for(auto it = it_range_begin; it != it_range_end; it++){
					objects_to_release.push_back(*it);
				}

				T_container::erase(it_range_begin, it_range_end);//erasing needs objects alive to call opCmp

				for(auto it = objects_to_release.begin(); it != objects_to_release.end(); it++){
					engine->ReleaseScriptObject(*it, objtype_content);
					delcount++;
				}
			}

			return delcount;
		}
	}
	template<class T_cond> static void Register_func_Erase_range_iterator(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_iterator_TT){}
	template<> static void Register_func_Erase_range_iterator<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "%s %s(const %s &in,const %s &in)", config::scriptname::t::size, config::scriptname::method::container::erase_iterator, n_iterator_TT, n_iterator_TT);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Erase_range_iterator<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}



	template<class T> aatc_iterator Find_iterator(void* value){}
	template<> aatc_iterator Find_iterator<aatc_Y>(void* value){
		if(!handlemode_directcomp){
			aatc_errorcheck_container_missingfunctions_operation_noret(CONTAINER_OPERATION::FIND, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::find)
				void* vthis = this;
				aatc_iterator result(&vthis, 0);
				result.SetToEnd();
				return result;
			}
		}
		if(handlemode){ value = *(void**)value; }

		auto it = T_container::find(value);

		void* vthis = this;
		aatc_iterator result(&vthis, 0);
			result.it = it;
			if(it == T_container::end()){
				result.cont = 0;
			}

		return result;
	}
	template<class T> aatc_iterator Find_iterator_generic(void* value){}
	template<> aatc_iterator Find_iterator_generic<aatc_Y>(void* value){
		if(!handlemode_directcomp){
			aatc_errorcheck_container_missingfunctions_operation_noret(CONTAINER_OPERATION::FIND, objtype_container->GetName(), objtype_content->GetName(), config::scriptname::method::container::find)
				void* vthis = this;
				aatc_iterator result(&vthis, 0);
				result.SetToEnd();
				return result;
			}
		}
		if(handlemode){ value = *(void**)value; }


		aatc_containerfunctor_equals eq(engine, this);
		aatc_containerfunctor_equals::findif_version eq_fi;
		eq_fi.f = &eq;
		eq_fi.target = value;

		//auto it = std::find(T_container::begin(), T_container::end(), value);
		//auto it = std::find(T_container::begin(), T_container::end(), value);
		auto it = std::find_if(T_container::begin(), T_container::end(), eq_fi);

		void* vthis = this;
		aatc_iterator result(&vthis, 0);
		result.it = it;
		if(it == T_container::end()){
			result.cont = 0;
		}

		return result;
	}
	template<class T_cond> static void Register_func_Find_iterator(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "%s %s(const T &in)", n_iterator_TT, config::scriptname::method::container::find_iterator);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Find_iterator_generic<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_Find_iterator<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "%s %s(const T &in)", n_iterator_TT, config::scriptname::method::container::find_iterator);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Find_iterator<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}


	template<class T> void Insert_iterator(const aatc_iterator& aatc_it, void* value){}
	template<> void Insert_iterator<aatc_Y>(const aatc_iterator& aatc_it, void* value){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
			if(iterator_safety_version != aatc_it.iterator_safety_version){
				common::errorprint::container::iterator_invalid();
				return;
			}
		#endif

		aatc_errorcheck_container_iterator_safety_version_Increment();

		void* final_value;

		if(handlemode){
			final_value = StoreHandle2(value);
		} else{
			final_value = engine->CreateScriptObjectCopy(value, objtype_content);
		}

		T_container::insert(aatc_it.it, final_value);
	}
	template<class T_cond> static void Register_func_Insert_iterator(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_iterator_TT){}
	template<> static void Register_func_Insert_iterator<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "void %s(const %s &in,const T &in)", config::scriptname::method::container::insert_iterator, n_iterator_TT);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_template, Insert_iterator<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}




};


template<class T_container, class T_traits>
void aatc_container_shared_1tp_template_Register(asIScriptEngine* engine, const char* n_container){
	int r = 0;

	char n_container_T[1000];
	sprintf_s(n_container_T,1000, "%s<T>", n_container);

	char n_container_class_T[1000];
	sprintf_s(n_container_class_T, 1000, "%s<class T>", n_container);

	char n_iterator[1000];
	sprintf_s(n_iterator, 1000, "%s%s", n_container, config::scriptname::iterator_suffix);

	char n_iterator_TT[1000];
	sprintf_s(n_iterator_TT, 1000, "%s<T>", n_iterator);

	char textbuf[1000];

	r = engine->RegisterObjectType(n_container_class_T, 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s@ f(int&in)", n_container_T);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_FACTORY, textbuf, asFUNCTIONPR(T_container::Factory, (asIObjectType*), T_container*), asCALL_CDECL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s@ f(int&in,const %s &in)", n_container_T, n_container_T);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_FACTORY, textbuf, asFUNCTIONPR(T_container::Factory_copy, (asIObjectType*,const T_container&), T_container*), asCALL_CDECL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s& opAssign(const %s &in)", n_container_T, n_container_T);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, operator=), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s& %s(%s@)", n_container_T, config::scriptname::method::container::swap, n_container_T);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Swap), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_ADDREF, "void f()", asMETHOD(T_container, refcount_Add), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_RELEASE, "void f()", asMETHOD(T_container, refcount_Release), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_SETGCFLAG, "void f()", asMETHODPR(T_container, SetGCFlag, (), void), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_GETGCFLAG, "bool f()", asMETHODPR(T_container, GetGCFlag, (), bool), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_GETREFCOUNT, "int f()", asMETHODPR(T_container, GetRefCount, (), int), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_ENUMREFS, "void f(int&in)", asMETHODPR(T_container, EnumReferences, (asIScriptEngine*), void), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHODPR(T_container, ReleaseAllReferences, (asIScriptEngine*), void), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(aatc_templatecallback_1tp), asCALL_CDECL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "void %s(bool)", config::scriptname::method::container::set_directcomp);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, SetDirectcomp), asCALL_THISCALL); assert(r >= 0);
	

	sprintf_s(textbuf, 1000, "%s %s(const T &in)", config::scriptname::t::size, config::scriptname::method::container::count);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Count), asCALL_THISCALL); assert(r >= 0);
	//r = engine->RegisterObjectMethod(n_container_T, "bool contains(const T &in)", asMETHOD(T_container, Contains), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "void %s()", config::scriptname::method::container::clear);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Clear), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "bool %s()", config::scriptname::method::container::empty);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Empty), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s %s()", config::scriptname::t::size, config::scriptname::method::container::size);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Size), asCALL_THISCALL); assert(r >= 0);

	T_container::Register_func_back_write<T_traits::trait_needfunc_BACK_WRITE>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_back_read<T_traits::trait_needfunc_BACK_READ>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_front_write<T_traits::trait_needfunc_FRONT_WRITE>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_front_read<T_traits::trait_needfunc_FRONT_READ>(engine, r, textbuf, n_container, n_container_T);
	//T_container::Register_func_erase_position<std::conditional<cond_NEEDFUNC_ERASE_POSITION, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_erase_value<T_traits::trait_needfunc_ERASE_VALUE>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_op_index<T_traits::trait_needfunc_OP_INDEX>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_reserve<T_traits::trait_needfunc_RESERVE>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_insert<T_traits::trait_needfunc_INSERT>(engine, r, textbuf, n_container, n_container_T);

	T_container::Register_func_sort_native<T_traits::trait_needfunc_SORT_NATIVE>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_sort_generic<T_traits::trait_needfunc_SORT_GENERIC>(engine, r, textbuf, n_container, n_container_T);

	T_container::Register_func_contains_native<T_traits::trait_needfunc_CONTAINS_NATIVE>(engine, r, textbuf, n_container, n_container_T);

	T_container::Register_func_erase_generic_index<T_traits::trait_needfunc_ERASE_GENERIC_INDEX>(engine, r, textbuf, n_container, n_container_T);
	T_container::Register_func_erase_generic_value<T_traits::trait_needfunc_ERASE_GENERIC_VALUE>(engine, r, textbuf, n_container, n_container_T);

	T_container::Register_func_insert_generic_index<T_traits::trait_needfunc_INSERT_GENERIC_INDEX>(engine, r, textbuf, n_container, n_container_T);



	T_container::aatc_iterator::Register<T_traits::trait_iterator_editable, T_traits::trait_iterator_const>(engine, n_iterator, n_container_T);

	sprintf_s(textbuf, 1000, "%s %s()", n_iterator_TT, config::scriptname::method::container::begin);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Begin), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s %s()", n_iterator_TT, config::scriptname::method::container::end);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, End), asCALL_THISCALL); assert(r >= 0);

	//sprintf_s(textbuf, 1000, "bool %s(const %s &in)", config::scriptname::method::container::erase_iterator, n_iterator_TT);
	//r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Erase_iterator), asCALL_THISCALL); assert(r >= 0);

	//sprintf_s(textbuf, 1000, "%s %s(const %s &in,const %s &in)", config::scriptname::t::size, config::scriptname::method::container::erase_iterator, n_iterator_TT, n_iterator_TT);
	//r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(T_container, Erase_iterator_range), asCALL_THISCALL); assert(r >= 0);



	T_container::Register_func_Erase_iterator<T_traits::trait_needfunc_ERASE_ITERATOR>(engine, r, textbuf, n_container, n_container_T, n_iterator_TT);
	T_container::Register_func_Erase_range_iterator<T_traits::trait_needfunc_ERASE_RANGE_ITERATOR>(engine, r, textbuf, n_container, n_container_T, n_iterator_TT);
	T_container::Register_func_Find_iterator<T_traits::trait_needfunc_FIND_ITERATOR>(engine, r, textbuf, n_container, n_container_T, n_iterator_TT);
	T_container::Register_func_Insert_iterator<T_traits::trait_needfunc_INSERT_ITERATOR>(engine, r, textbuf, n_container, n_container_T, n_iterator_TT);
}





END_AS_NAMESPACE
#endif