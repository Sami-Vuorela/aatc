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


#ifndef _includedh_aatc_shared_1t_tempspec
#define _includedh_aatc_shared_1t_tempspec



#include "aatc_common.hpp"



BEGIN_AS_NAMESPACE




/*!\brief Internal template monster

*/
template<class T_container, class T_content> class aatc_container_shared_1tp_tempspec : public aatc_container_base,
																						public T_container,
																						public aatc_refcounted{
public:
	typename typedef T_container::iterator iteratortype;

	//used to return something when script exceptions happen and the proper result cannot be returned
	static T_content defaultvalue;

	//used for error messages to retrieve names without sacrificing runtime performance while errors dont happen
	static std::string staticname_container;
	static std::string staticname_content;

	aatc_container_shared_1tp_tempspec(){
		auto* ctx = asGetActiveContext();
		if(ctx){ engine = ctx->GetEngine(); }
	}
	aatc_container_shared_1tp_tempspec(const aatc_container_shared_1tp_tempspec& other) :
		T_container(other)
	{
		engine = other.engine;
	}
	virtual ~aatc_container_shared_1tp_tempspec(){}

	void operator=(const aatc_container_shared_1tp_tempspec& other){
		T_container::operator=(other);
		aatc_errorcheck_container_iterator_safety_version_Increment();
	}
	void swap(aatc_container_shared_1tp_tempspec& other){
		T_container::swap(static_cast<T_container&>(other));
		aatc_errorcheck_container_iterator_safety_version_Increment();
		#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
			other.iterator_safety_version++;
		#endif
	}

	aatc_type_sizetype Count(const T_content& value){ return (aatc_type_sizetype)(std::count(T_container::begin(), T_container::end(), value)); }
	bool Contains_generic(const T_content& value){ return Count(value) > 0; }

	void Clear(){
		T_container::clear();
		aatc_errorcheck_container_iterator_safety_version_Increment();
	}
	bool Empty(){ return T_container::empty(); }
	aatc_type_sizetype Size(){ return (aatc_type_sizetype)(T_container::size()); }


	template<class T> void Push_Back(const T_content& value){}
	template<class T> void Pop_Back(){}
	template<> void Push_Back<aatc_Y>(const T_content& value){
		T_container::push_back(value);
		aatc_errorcheck_container_iterator_safety_version_Increment();
	}
	template<> void Pop_Back<aatc_Y>(){
		T_container::pop_back();
		aatc_errorcheck_container_iterator_safety_version_Increment();
	}
	template<class T_cond> static void Register_func_back_write(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_back_write<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(const %s &in)", config::scriptname::method::container::push_back, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Push_Back<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s()", config::scriptname::method::container::pop_back);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Pop_Back<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> T_content& Back(){}
	template<> T_content& Back<aatc_Y>(){
		aatc_errorcheck_container_access_empty_retdefault(staticname_container.c_str(), staticname_content.c_str(), config::scriptname::method::container::back)
		return T_container::back();
	}
	template<class T_cond> static void Register_func_back_read(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_back_read<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "%s& %s()", n_content, config::scriptname::method::container::back);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Back<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Push_Front(const T_content& value){}
	template<class T> void Pop_Front(){}
	template<> void Push_Front<aatc_Y>(const T_content& value){
		T_container::push_front(value);
		aatc_errorcheck_container_iterator_safety_version_Increment();
	}
	template<> void Pop_Front<aatc_Y>(){
		T_container::pop_front();
		aatc_errorcheck_container_iterator_safety_version_Increment();
	}
	template<class T_cond> static void Register_func_front_write(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_front_write<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(const %s &in)", config::scriptname::method::container::push_front, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Push_Front<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s()", config::scriptname::method::container::pop_front);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Pop_Front<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> T_content& Front(){}
	template<> T_content& Front<aatc_Y>(){
		aatc_errorcheck_container_access_empty_retdefault(staticname_container.c_str(), staticname_content.c_str(), config::scriptname::method::container::front)
		return T_container::front();
	}
	template<class T_cond> static void Register_func_front_read(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_front_read<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "%s& %s()", n_content, config::scriptname::method::container::front);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Front<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	//template<class T> void Erase_position(aatc_type_sizetype position){}
	//template<> void Erase_position<aatc_Y>(aatc_type_sizetype position){
	//	if((position > -1) && (std::size_t(position) < T_container::size())){
	//		typename T_container::iterator ii = T_container::begin() + std::size_t(position);
	//		T_container::erase(ii);
	//	}
	//}
	//template<class T_cond> static void Register_func_erase_position(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	//template<> static void Register_func_erase_position<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
	//	sprintf_s(textbuf,1000, "void erase(%s)", aatc_name_script_sizetype);
	//	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Erase_position<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	//}

	template<class T> void Erase_value(const T_content& value){}
	template<> void Erase_value<aatc_Y>(const T_content& value){
		T_container::erase(value);
		aatc_errorcheck_container_iterator_safety_version_Increment();
	}
	template<class T_cond> static void Register_func_erase_value(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_erase_value<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(const %s &in)", config::scriptname::method::container::erase, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Erase_value<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}



	template<class T> T_content& op_index(aatc_type_sizetype index){}
	template<> T_content& op_index<aatc_Y>(aatc_type_sizetype index){
		aatc_errorcheck_container_access_empty_retdefault(staticname_container.c_str(), staticname_content.c_str(), "opIndex")
			aatc_errorcheck_container_access_bounds_retdefault(std::size_t(index), T_container::size(), staticname_container.c_str(), staticname_content.c_str(), "opIndex")
		return T_container::operator[](index);
	}
	template<class T_cond> static void Register_func_op_index(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_op_index<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf,1000, "%s& opIndex(%s)", n_content, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, op_index<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Reserve(aatc_type_sizetype count){}
	template<> void Reserve<aatc_Y>(aatc_type_sizetype count){
		T_container::reserve(count);
		aatc_errorcheck_container_iterator_safety_version_Increment();
	}
	template<class T_cond> static void Register_func_reserve(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_reserve<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(%s)", config::scriptname::method::container::reserve, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Reserve<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Insert(const T_content& value){}
	template<> void Insert<aatc_Y>(const T_content& value){
		T_container::insert(value);
		aatc_errorcheck_container_iterator_safety_version_Increment();
	}
	template<class T_cond> static void Register_func_insert(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_insert<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(const %s &in)", config::scriptname::method::container::insert, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Insert<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}


	template<class T> void Sort_native(bool ascending){}
	template<> void Sort_native<aatc_Y>(bool ascending){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if (ascending){
			T_container::sort(std::less<T_content>());
		}else{
			//T_container::sort(aatc_functor_notcmp<T_content, std::less<T_content>>());

			/*
				std::list didn't like aatc_functor_notcmp with primitives in debug mode for some reason
				so we'll have to use some black majicks
			*/
			T_container::sort(
				std::conditional<
					std::is_class<T_content>::value,
					aatc_functor_notcmp<T_content, std::less<T_content>>,
					std::greater<T_content>
				>::type()
			);
		}
	}
	template<class T_cond> static void Register_func_sort_native(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_sort_native<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(bool ascending = true)", config::scriptname::method::container::sort);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Sort_native<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> void Sort_generic(bool ascending){}
	template<> void Sort_generic<aatc_Y>(bool ascending){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if (ascending){
			std::sort(T_container::begin(), T_container::end(), std::less<T_content>());
		}else{
			std::sort(T_container::begin(), T_container::end(), aatc_functor_notcmp<T_content, std::less<T_content>>());
		}
	}
	template<class T_cond> static void Register_func_sort_generic(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_sort_generic<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(bool ascending = true)", config::scriptname::method::container::sort);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Sort_generic<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}

	template<class T> bool Contains_native(const T_content& value){}
	template<> bool Contains_native<aatc_Y>(const T_content& value){
		T_container::const_iterator it = T_container::find(value);
		return it != T_container::end();
	}
	template<class T_cond> static void Register_func_contains_native(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "bool %s(const %s &in)", config::scriptname::method::container::contains, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Contains_generic), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_contains_native<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "bool %s(const %s &in)", config::scriptname::method::container::contains, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Contains_native<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
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

	template<class T_is_native> void Insert_generic_index_before(int index, const T_content& value){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if((index > -1) && (index <= T_container::size())){
			T_container::iterator ii = T_container::begin();
			{_functor_iterator_setindex<T_is_native> ff; ff(ii, index); }

			insert(ii, value);
		}
	}
	template<class T_is_native> static void Register_func_insert_generic_index(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(%s,const %s &in)", config::scriptname::method::container::insert_index_before, aatc_name_script_sizetype, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Insert_generic_index_before<T_is_native>), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_insert_generic_index<aatc_NULLCLASS>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}


	template<class T_is_native> void Erase_generic_index(int position){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		if((position > -1) && (position < T_container::size())){
			T_container::iterator ii = T_container::begin();

			{_functor_iterator_setindex<T_is_native> ff; ff(ii, position); }

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

				T_container::erase(ii_range_begin, ii_range_end);
			}
		}
	}

	template<class T_is_native> static void Register_func_erase_generic_index(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(%s)", config::scriptname::method::container::erase_index, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Erase_generic_index<T_is_native>), asCALL_THISCALL); assert(r >= 0);
		sprintf_s(textbuf, 1000, "void %s(%s,%s)", config::scriptname::method::container::erase_index, aatc_name_script_sizetype, aatc_name_script_sizetype);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Erase_generic_index_range<T_is_native>), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_erase_generic_index<aatc_NULLCLASS>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}



	template<class T_existence> void Erase_generic_value(const T_content& value, bool all){
		aatc_errorcheck_container_iterator_safety_version_Increment();

		bool continuu = 1;
		while(continuu){
			continuu = 0;
			bool found = 0;

			T_container::iterator it = T_container::begin();
			T_container::iterator itend = T_container::end();

			for(; it != itend; it++){
				if(*it == value){
					found = 1;
					T_container::erase(it);
					break;
				}
			}
			if(empty()){ break; }

			if(found && all){ continuu = 1; }
		}
	}

	template<class T_existence> static void Register_func_erase_generic_value(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){}
	template<> static void Register_func_erase_generic_value<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content){
		sprintf_s(textbuf, 1000, "void %s(const %s &in value,bool all = false)", config::scriptname::method::container::erase_value, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Erase_generic_value<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}





	class aatc_iterator : public aatc_iterator_base{
	public:
		aatc_container_shared_1tp_tempspec* host;
		typename aatc_container_shared_1tp_tempspec::iterator it;
		typename aatc_container_shared_1tp_tempspec::iterator it_end;

		aatc_iterator(){}
		aatc_iterator(aatc_container_shared_1tp_tempspec* _host) :
			host(_host)
		{
			firstt = 1;
			if(host->empty()){
				cont = 0;

				it = host->end();
				it_end = host->end();
			} else{
				it = host->begin();
				it_end = host->end();
				cont = 1;
			}

			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				iterator_safety_version = host->iterator_safety_version;
			#endif
		}
		aatc_iterator(const aatc_iterator& other) :
			aatc_iterator_base(other),

			host(other.host),
			it(other.it),
			it_end(other.it_end)
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

			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				iterator_safety_version = other.iterator_safety_version;
			#endif

			return *this;
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

		template<class T> const T_content& Current_get(){}
		template<class T> void Current_set(const T_content& newval){}
		template<> const T_content& Current_get<aatc_Y>(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				if(iterator_safety_version != host->iterator_safety_version){
					aatc_errorprint_iterator_container_modified();
					return host->defaultvalue;
				}
			#endif
			return *it;
		}
		template<> void Current_set<aatc_Y>(const T_content& newval){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
						if(iterator_safety_version != host->iterator_safety_version){
							aatc_errorprint_iterator_container_modified();
							return;
						}
			#endif
			*it = newval;
		}

		template<class T> T_content& Current(){}
		template<> T_content& Current<aatc_Y>(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
						if(iterator_safety_version != host->iterator_safety_version){
							aatc_errorprint_iterator_container_modified();
							return host->defaultvalue;
						}
			#endif
			return *it;
		}
		template<class T_cond> static void Register_func_current(asIScriptEngine* engine, int& r, char* textbuf, const char* n_iterator, const char* n_content){}
		template<> static void Register_func_current<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_iterator, const char* n_content){
			sprintf_s(textbuf, 1000, "%s& %s()", n_content, config::scriptname::method::iterator::access_function);
			r = engine->RegisterObjectMethod(n_iterator, textbuf, asMETHOD(aatc_iterator, Current<aatc_Y>), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "%s& get_%s()", n_content, config::scriptname::method::iterator::access_property);
			r = engine->RegisterObjectMethod(n_iterator, textbuf, asMETHOD(aatc_iterator, Current_get<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
			sprintf_s(textbuf, 1000, "void set_%s(const %s &in)", config::scriptname::method::iterator::access_property, n_content);
			r = engine->RegisterObjectMethod(n_iterator, textbuf, asMETHOD(aatc_iterator, Current_set<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		}

		template<class T> const T_content& Current_const(){}
		template<> const T_content& Current_const<aatc_Y>(){
			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
						if(iterator_safety_version != host->iterator_safety_version){
							aatc_errorprint_iterator_container_modified();
							return host->defaultvalue;
						}
			#endif
			return *it;
		}
		template<class T_cond> static void Register_func_current_const(asIScriptEngine* engine, int& r, char* textbuf, const char* n_iterator, const char* n_content){}
		template<> static void Register_func_current_const<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_iterator, const char* n_content){
			sprintf_s(textbuf, 1000, "const %s& %s()", n_content, config::scriptname::method::iterator::access_function);
			r = engine->RegisterObjectMethod(n_iterator, textbuf, asMETHOD(aatc_iterator, Current_const<aatc_Y>), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "const %s& get_%s()", n_content, config::scriptname::method::iterator::access_property);
			r = engine->RegisterObjectMethod(n_iterator, textbuf, asMETHOD(aatc_iterator, Current_get<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
		}

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

		template<class cond_EDITABLE = aatc_Y, class cond_CONST = aatc_N> static void Register(asIScriptEngine* engine, const char* n_iterator, const char* n_content, const char* n_containerTcontentT){
			int r = 0;
			char textbuf[1000];

			r = engine->RegisterObjectType(n_iterator, sizeof(aatc_iterator), asOBJ_VALUE | asGetTypeTraits<aatc_iterator>()); assert(r >= 0);

			sprintf_s(textbuf, 1000, "void f()");
			r = engine->RegisterObjectBehaviour(n_iterator, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(aatc_reghelp_constructor<aatc_iterator>), asCALL_CDECL_OBJLAST); assert(r >= 0);
			sprintf_s(textbuf, 1000, "void f(%s@)", n_containerTcontentT);
			r = engine->RegisterObjectBehaviour(n_iterator, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(aatc_reghelp_constructor_1_param<aatc_iterator, aatc_container_shared_1tp_tempspec*>), asCALL_CDECL_OBJLAST); assert(r >= 0);
			sprintf_s(textbuf, 1000, "void f(const %s &in)", n_iterator);
			r = engine->RegisterObjectBehaviour(n_iterator, asBEHAVE_CONSTRUCT, textbuf, asFunctionPtr(aatc_reghelp_constructor_copy<aatc_iterator, aatc_iterator>), asCALL_CDECL_OBJLAST); assert(r >= 0);
			r = engine->RegisterObjectBehaviour(n_iterator, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(aatc_reghelp_generic_destructor<aatc_iterator>), asCALL_CDECL_OBJLAST); assert(r >= 0);

			aatc_iterator::Register_func_current<cond_EDITABLE>(engine, r, textbuf, n_iterator, n_content);
			aatc_iterator::Register_func_current_const<cond_CONST>(engine, r, textbuf, n_iterator, n_content);

			r = engine->RegisterObjectMethod(n_iterator, "bool next()", asMETHOD(aatc_iterator, Next), asCALL_THISCALL); assert(r >= 0);
			r = engine->RegisterObjectMethod(n_iterator, "bool opPreInc()", asMETHOD(aatc_iterator, Next), asCALL_THISCALL); assert(r >= 0);
			r = engine->RegisterObjectMethod(n_iterator, "bool opPostInc()", asMETHOD(aatc_iterator, Next), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "%s& opAssign(const %s &in)", n_iterator, n_iterator);
			r = engine->RegisterObjectMethod(n_iterator, textbuf, asMETHODPR(aatc_iterator, operator=, (const aatc_iterator&), aatc_iterator&), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "bool opEquals(const %s &in)", n_iterator);
			r = engine->RegisterObjectMethod(n_iterator, textbuf, asMETHOD(aatc_iterator, operator==), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "bool %s()", config::scriptname::method::iterator::is_end);
			r = engine->RegisterObjectMethod(n_iterator, textbuf, asMETHOD(aatc_iterator, IsEnd), asCALL_THISCALL); assert(r >= 0);

			sprintf_s(textbuf, 1000, "bool %s()", config::scriptname::method::iterator::is_valid);
			r = engine->RegisterObjectMethod(n_iterator, textbuf, asMETHOD(aatc_iterator, IsValid), asCALL_THISCALL); assert(r >= 0);
		}
	};



	aatc_iterator Begin(){
		return aatc_iterator(this);
	}
	aatc_iterator End(){
		aatc_iterator result(this);
		result.SetToEnd();
		return result;
	}




	template<class T> bool Erase_iterator(const aatc_iterator& aatc_it){}
	template<> bool Erase_iterator<aatc_Y>(const aatc_iterator& aatc_it){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				if(iterator_safety_version != aatc_it.iterator_safety_version){
					aatc_errorprint_container_iterator_invalid();
					return 0;
				}
		#endif

		aatc_errorcheck_container_iterator_safety_version_Increment();

		T_container::iterator it = aatc_it.it;

		if(T_container::empty()){
			return 0;
		} else{
			if(it == T_container::end()){
				return 0;
			}

			T_container::erase(it);
			return 1;
		}
	}
	template<class T_cond> static void Register_func_Erase_iterator(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content, const char* n_iterator_TT){}
	template<> static void Register_func_Erase_iterator<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "bool %s(const %s &in)", config::scriptname::method::container::erase_iterator, n_iterator_TT);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Erase_iterator<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}


	template<class T> aatc_type_sizetype Erase_iterator_range(const aatc_iterator& aatc_it_range_begin, const aatc_iterator& aatc_it_range_end){}
	template<> aatc_type_sizetype Erase_iterator_range<aatc_Y>(const aatc_iterator& aatc_it_range_begin, const aatc_iterator& aatc_it_range_end){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
			if((iterator_safety_version != aatc_it_range_begin.iterator_safety_version) || (iterator_safety_version != aatc_it_range_end.iterator_safety_version)){
				aatc_errorprint_container_iterator_invalid();
				return 0;
			}
		#endif

		aatc_errorcheck_container_iterator_safety_version_Increment();

		T_container::iterator it_range_begin = aatc_it_range_begin.it;
		T_container::iterator it_range_end = aatc_it_range_end.it;

		if(it_range_begin == it_range_end){
			return 0;
		} else{
			aatc_type_sizetype delcount = (aatc_type_sizetype)std::distance(it_range_begin, it_range_end);
			T_container::erase(it_range_begin, it_range_end);
			return delcount;
		}
	}
	template<class T_cond> static void Register_func_Erase_iterator_range(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content, const char* n_iterator_TT){}
	template<> static void Register_func_Erase_iterator_range<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "%s %s(const %s &in,const %s &in)", aatc_name_script_sizetype, config::scriptname::method::container::erase_iterator, n_iterator_TT, n_iterator_TT);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Erase_iterator_range<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}


	template<class T> aatc_iterator Find_iterator(const T_content& value){}
	template<> aatc_iterator Find_iterator<aatc_Y>(const T_content& value){
		auto it = T_container::find(value);

		aatc_iterator result(this);
		result.it = it;

		if(it == T_container::end()){
			result.cont = 0;
		}

		return result;
	}
	template<class T> aatc_iterator Find_iterator_generic(const T_content& value){}
	template<> aatc_iterator Find_iterator_generic<aatc_Y>(const T_content& value){
		auto it = std::find(T_container::begin(), T_container::end(), value);

		aatc_iterator result(this);
		result.it = it;

		if(it == T_container::end()){
			result.cont = 0;
		}

		return result;
	}
	template<class T_cond> static void Register_func_Find_iterator(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "%s %s(const %s &in)", n_iterator_TT, config::scriptname::method::container::find_iterator, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Find_iterator_generic<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}
	template<> static void Register_func_Find_iterator<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "%s %s(const %s &in)", n_iterator_TT, config::scriptname::method::container::find_iterator, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Find_iterator<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}




	template<class T> void Insert_iterator(const aatc_iterator& aatc_it, const T_content& value){}
	template<> void Insert_iterator<aatc_Y>(const aatc_iterator& aatc_it, const T_content& value){
		#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				if(iterator_safety_version != aatc_it.iterator_safety_version){
					aatc_errorprint_container_iterator_invalid();
					return;
				}
		#endif

		aatc_errorcheck_container_iterator_safety_version_Increment();

		T_container::insert(aatc_it.it, value);
	}
	template<class T_cond> static void Register_func_Insert_iterator(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content, const char* n_iterator_TT){}
	template<> static void Register_func_Insert_iterator<aatc_Y>(asIScriptEngine* engine, int& r, char* textbuf, const char* n_container, const char* n_container_T, const char* n_content, const char* n_iterator_TT){
		sprintf_s(textbuf, 1000, "void %s(const %s &in,const %s &in)", config::scriptname::method::container::insert_iterator, n_iterator_TT, n_content);
		r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(aatc_container_shared_1tp_tempspec, Insert_iterator<aatc_Y>), asCALL_THISCALL); assert(r >= 0);
	}
};



template<class T_container, class T_content> T_content aatc_container_shared_1tp_tempspec<T_container, T_content>::defaultvalue = T_content();
template<class T_container, class T_content> std::string aatc_container_shared_1tp_tempspec<T_container, T_content>::staticname_container = "test_container";
template<class T_container, class T_content> std::string aatc_container_shared_1tp_tempspec<T_container, T_content>::staticname_content = "test_content";



template<class dt_container, class dt_content, bool GOTFUNC_EQUALS = 0, bool GOTFUNC_LESS = 0, bool GOTFUNC_HASH = 0, class T_traits>
void aatc_container_shared_1tp_tempspec_Register(asIScriptEngine* engine, const char* n_container, const char* n_content){
	int r = 0;
	char textbuf[1000];


	char n_container_T[1000];
	sprintf_s(n_container_T, 1000, "%s<%s>", n_container, n_content);

	char n_iterator[1000];
	sprintf_s(n_iterator, 1000, "%s%s", n_container, config::scriptname::iterator_suffix);

	char n_iterator_TT[1000];
	sprintf_s(n_iterator_TT, 1000, "%s<%s>", n_iterator, n_content);

	r = engine->RegisterObjectType(n_container_T, 0, asOBJ_REF); assert(r >= 0);

	sprintf_s(textbuf,1000, "%s@ f()", n_container_T);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_FACTORY, textbuf, asFUNCTIONPR(dt_container::Factory, (), dt_container*), asCALL_CDECL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s@ f(const %s &in)", n_container_T, n_container_T);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_FACTORY, textbuf, asFUNCTIONPR(dt_container::Factory_copy, (const dt_container&), dt_container*), asCALL_CDECL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s& opAssign(const %s &in)", n_container_T, n_container_T);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, operator=), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s& %s(%s@)", n_container_T, config::scriptname::method::container::swap, n_container_T);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, Swap), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_ADDREF, "void f()", asMETHOD(dt_container, refcount_Add), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(n_container_T, asBEHAVE_RELEASE, "void f()", asMETHOD(dt_container, refcount_Release), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s %s(const %s &in)", aatc_name_script_sizetype, config::scriptname::method::container::count, n_content);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, Count), asCALL_THISCALL); assert(r >= 0);
	//sprintf_s(textbuf,1000, "bool contains(const %s &in)", n_content);
	//r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, Contains), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "void %s()", config::scriptname::method::container::clear);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, Clear), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "bool %s()", config::scriptname::method::container::empty);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, Empty), asCALL_THISCALL); assert(r >= 0);
	sprintf_s(textbuf, 1000, "%s %s()", aatc_name_script_sizetype, config::scriptname::method::container::size);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, Size), asCALL_THISCALL); assert(r >= 0);

	dt_container::Register_func_back_write<T_traits::trait_needfunc_BACK_WRITE>(engine, r, textbuf, n_container, n_container_T, n_content);
	dt_container::Register_func_back_read<T_traits::trait_needfunc_BACK_READ>(engine, r, textbuf, n_container, n_container_T, n_content);
	dt_container::Register_func_front_write<T_traits::trait_needfunc_FRONT_WRITE>(engine, r, textbuf, n_container, n_container_T, n_content);
	dt_container::Register_func_front_read<T_traits::trait_needfunc_FRONT_READ>(engine, r, textbuf, n_container, n_container_T, n_content);

	//dt_container::Register_func_erase_position<std::conditional<cond_NEEDFUNC_ERASE_POSITION, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T, n_content);
	dt_container::Register_func_op_index<T_traits::trait_needfunc_OP_INDEX>(engine, r, textbuf, n_container, n_container_T, n_content);
	dt_container::Register_func_reserve<T_traits::trait_needfunc_RESERVE>(engine, r, textbuf, n_container, n_container_T, n_content);

	//functions that require a == operator
	dt_container::Register_func_erase_value<std::conditional<GOTFUNC_EQUALS && std::is_same<T_traits::trait_needfunc_ERASE_VALUE, aatc_Y>::value, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T, n_content);

	//functions that require a < operator
	dt_container::Register_func_insert<std::conditional<GOTFUNC_LESS && std::is_same<T_traits::trait_needfunc_INSERT, aatc_Y>::value, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T, n_content);
	dt_container::Register_func_sort_native<std::conditional<GOTFUNC_LESS && std::is_same<T_traits::trait_needfunc_SORT_NATIVE, aatc_Y>::value, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T, n_content);
	dt_container::Register_func_sort_generic<std::conditional<GOTFUNC_LESS && std::is_same<T_traits::trait_needfunc_SORT_GENERIC, aatc_Y>::value, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T, n_content);
	dt_container::Register_func_contains_native<std::conditional<GOTFUNC_LESS && std::is_same<T_traits::trait_needfunc_CONTAINS_NATIVE, aatc_Y>::value, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T, n_content);

	dt_container::Register_func_erase_generic_index<T_traits::trait_needfunc_ERASE_GENERIC_INDEX>(engine, r, textbuf, n_container, n_container_T, n_content);
	dt_container::Register_func_erase_generic_value<std::conditional<GOTFUNC_EQUALS && std::is_same<T_traits::trait_needfunc_ERASE_GENERIC_VALUE, aatc_Y>::value, aatc_Y, aatc_N>::type>(engine, r, textbuf, n_container, n_container_T, n_content);

	dt_container::Register_func_insert_generic_index<T_traits::trait_needfunc_INSERT_GENERIC_INDEX>(engine, r, textbuf, n_container, n_container_T, n_content);





	dt_container::aatc_iterator::Register<T_traits::trait_iterator_editable, T_traits::trait_iterator_const>(engine, n_iterator_TT, n_content, n_container_T);

	sprintf_s(textbuf, 1000, "%s %s()", n_iterator_TT, config::scriptname::method::container::begin);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, Begin), asCALL_THISCALL); assert(r >= 0);

	sprintf_s(textbuf, 1000, "%s %s()", n_iterator_TT, config::scriptname::method::container::end);
	r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, End), asCALL_THISCALL); assert(r >= 0);

	dt_container::Register_func_Erase_iterator<T_traits::trait_needfunc_ERASE_ITERATOR>(engine, r, textbuf, n_container, n_container_T, n_content, n_iterator_TT);
	dt_container::Register_func_Erase_iterator_range<T_traits::trait_needfunc_ERASE_RANGE_ITERATOR>(engine, r, textbuf, n_container, n_container_T, n_content, n_iterator_TT);
	dt_container::Register_func_Find_iterator<T_traits::trait_needfunc_FIND_ITERATOR>(engine, r, textbuf, n_container, n_container_T, n_content, n_iterator_TT);
	dt_container::Register_func_Insert_iterator<T_traits::trait_needfunc_INSERT_ITERATOR>(engine, r, textbuf, n_container, n_container_T, n_content, n_iterator_TT);

	//sprintf_s(textbuf, 1000, "bool %s(const %s &in)", config::scriptname::method::container::erase_iterator, n_iterator_TT);
	//r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, Erase_iterator), asCALL_THISCALL); assert(r >= 0);
	//sprintf_s(textbuf, 1000, "bool %s(const %s &in,const %s &in)", config::scriptname::method::container::erase_iterator, n_iterator_TT, n_iterator_TT);
	//r = engine->RegisterObjectMethod(n_container_T, textbuf, asMETHOD(dt_container, Erase_iterator_range), asCALL_THISCALL); assert(r >= 0);
}





END_AS_NAMESPACE
#endif