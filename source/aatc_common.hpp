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


#ifndef _includedh_aatc_common
#define _includedh_aatc_common

//#include "cm/core/thirdparty/tp_angelscript.hpp"

#include "aatc.hpp"



#if !aatc_CONFIG_USE_BOOST
	/*!\brief Minimal spinlock class*/
	class aatc_std_Spinlock{
	public:
		//std::atomic<int> m_state = ATOMIC_VAR_INIT(1);//this thing useful?
		std::atomic<int> state;
		aatc_std_Spinlock();
		void lock();
		void unlock();
	};
#endif

#if aatc_CONFIG_USE_ASADDON_REF
	#if AS_USE_NAMESPACE
		typedef AngelScript::CScriptHandle aatc_ait_ref;
	#else
		typedef CScriptHandle aatc_ait_ref;
	#endif
	

	//for map / set
	template<> class std::less<aatc_ait_ref>{
	public:
		bool operator()(const aatc_ait_ref& lhs, const aatc_ait_ref& rhs) const{
			return lhs.m_ref < rhs.m_ref;
		}
	};
	template<> class std::greater<aatc_ait_ref>{
	public:
		bool operator()(const aatc_ait_ref& lhs, const aatc_ait_ref& rhs) const{
			return lhs.m_ref > rhs.m_ref;
		}
	};
	//template<> class std::greater<aatc_ait_ref>{
	//public:
	//	bool operator()(const aatc_ait_ref& lhs, const aatc_ait_ref& rhs) const{
	//		return lhs.m_ref > rhs.m_ref;
	//	}
	//};
#endif



BEGIN_AS_NAMESPACE

//doxygen skip
#ifndef DOXYGEN_SHOULD_SKIP_THIS

	//this beautiful creation makes std::list<int>::sort cry salty compiler tears in debug mode for some reason
	template<class T,class T_other_functor> class aatc_functor_notcmp{
	public:
		bool operator()(const T& lhs, const T& rhs) const{
			return !(T_other_functor()(lhs, rhs));
		}
	};
	template<class T, class T_other_functor> class aatc_functor_notcmp_persistent_noconst : public T_other_functor{
	public:
		aatc_functor_notcmp_persistent_noconst(const T_other_functor& base) :
			T_other_functor(base)
		{}
		bool operator()(T& lhs, T& rhs){
			return !(T_other_functor::operator()(lhs, rhs));
		}
	};

#endif
//doxygen skip

	/*!\brief The functor that unordered containers use for hashing.
	
		You should specialize this for your c++ classes before making
		tempspecs of unordered containers of them.
	*/
	template<class T> class aatc_functor_hash{
	public:
		aatc_hash_type operator()(const T& a) const{
			return (aatc_hash_type)a;
		}
	};
	template<> class aatc_functor_hash<aatc_type_float32>{
	public:
		aatc_hash_type operator()(const aatc_type_float32& a) const;
	};
	template<> class aatc_functor_hash<aatc_type_float64>{
	public:
		aatc_hash_type operator()(const aatc_type_float64& a) const;
	};
	template<> class aatc_functor_hash<aatc_type_string>{
	public:
		aatc_hash_type operator()(const aatc_type_string& a) const;
	};
	#if aatc_CONFIG_USE_ASADDON_REF
		template<> class aatc_functor_hash<aatc_ait_ref>{
		public:
			aatc_hash_type operator()(const aatc_ait_ref& a) const;
		};
	#endif

	/*
		Use these to register your c++ classes for hashing in containers that don't have a tempspec available.
		Because you can't register functors with RegisterObjectMethod.
	*/
	template<typename T> aatc_hash_type aatc_func_hash_value(const T& me){ aatc_functor_hash<T> functor; return functor(me); }

	template<typename T_your_cpp_type> void aatc_Register_aatc_func_hash_value(asIScriptEngine* engine, const char* name_your_type_in_script){
		char textbuf[1000];
		sprintf_s(textbuf, 1000, "%s %s()", aatc_hash_type_scriptname_actual, aatc_name_script_requiredmethod_hash);
		int error = engine->RegisterObjectMethod(name_your_type_in_script, textbuf, asFUNCTION(aatc_func_hash_value<T_your_cpp_type>), asCALL_CDECL_OBJLAST); assert(error >= 0);
	}


/*
	Enum listing the different container operations
	that require script functions to be called.
	Used mostly for error checking.
*/
enum aatc_CONTAINER_OPERATION{
	COUNT,
	ERASE_VALUE,
	INSERT,
	SORT,
	CONTAINS_NATIVE,
	REMOVE,
	FIND,
	ERASE_GENERIC_VALUE
};

typedef aatc_type_uint32 aatc_container_operations_bitmask_type;

class aatc_template_specific_storage;
class aatc_containertype_specific_storage;
class aatc_engine_level_storage;


/*!\brief Stores the function pointers of a specific container<datatype>. One per datatype per container type per engine.*/
class aatc_template_specific_storage{
public:
	aatc_containertype_specific_storage* ctss;

	asIScriptFunction* func_equals;
	asIScriptFunction* func_cmp;
	asIScriptFunction* func_hash;

	aatc_container_operations_bitmask_type missing_functions;

	aatc_template_specific_storage(aatc_containertype_specific_storage* ctss,aatc_type_uint32 id);
};

/*!\brief Stores dataz about a specific container type. One per container type per engine.*/
class aatc_containertype_specific_storage{
public:
	aatc_engine_level_storage* els;
	aatc_type_uint32 container_id;

	//typedef aatc_ait_storage_map<uint32, aatc_iterator_storage*> tmap_is;
	//typedef aatc_ait_storage_pair<uint32, aatc_iterator_storage*> tpair_is;
	//tmap_is iterator_storages;
	//aatc_ait_fastlock iterator_storages_lock;

	typedef aatc_ait_storage_map<aatc_type_uint32, aatc_template_specific_storage*> tmap_tss;
	typedef aatc_ait_storage_pair<aatc_type_uint32, aatc_template_specific_storage*> tpair_tss;
	tmap_tss template_specific_storages;
	aatc_ait_fastlock template_specific_storages_lock;

	//aatc_iterator_storage* GetIteratorStorage(uint32 id);
	aatc_template_specific_storage* GetTemplateSpecificStorage(aatc_type_uint32 id);

	aatc_containertype_specific_storage(aatc_engine_level_storage* els, aatc_type_uint32 container_id);
	~aatc_containertype_specific_storage();
};


/*!\brief Stores dataz about aatc containers. One for each engine.*/
class aatc_engine_level_storage{
public:
	asIScriptEngine* engine;

	typedef aatc_ait_storage_map<aatc_type_uint32, aatc_containertype_specific_storage*> tmap_ctss;
	typedef aatc_ait_storage_pair<aatc_type_uint32, aatc_containertype_specific_storage*> tpair_ctss;
	tmap_ctss containertype_specific_storages;
	aatc_ait_fastlock containertype_specific_storages_lock;

	std::vector<asIScriptContext*> context_cache;
	aatc_ait_fastlock context_cache_lock;
	

	asIObjectType* objtype_tempcont_list;

	aatc_engine_level_storage(asIScriptEngine* engine);
	~aatc_engine_level_storage();

	aatc_containertype_specific_storage* GetContainerTypeSpecificStorage(aatc_type_uint32 id);

	asIScriptContext* contextcache_Get();
	void contextcache_Return(asIScriptContext* a);

	void Clean();
};

//convenience, uses engine level storage, aatc must be initialized
asIScriptContext* aatc_contextcache_Get();
void aatc_contextcache_Return(asIScriptContext* c);



//doxygen skip
#ifndef DOXYGEN_SHOULD_SKIP_THIS

template<class T> void aatc_reghelp_constructor(void *memory){ new(memory)T(); }
template<class T, class T_other> void aatc_reghelp_constructor_copy(const T_other& other, void *memory){ new(memory)T(other); }
template<class T, class T_input1> void aatc_reghelp_constructor_1_param(T_input1 x, void *memory){ new(memory)T(x); }
template<typename T_out, typename T_host> T_out aatc_reghelp_construct_hosted(T_host cont){
	return T_out(cont);
}
template<class T> void aatc_reghelp_constructor_template_default(asIObjectType* objtype,void *memory){ new(memory)T(); }

template<class T> void aatc_reghelp_generic_destructor(void *memory){
	((T*)memory)->~T();
}

#endif
//doxygen skip



//refcounted and gc basetype
//refcounted and gc basetype

/*!\brief Basetype for script refcounted c++ objects to derive from.*/
class aatc_refcounted{
public:
	int refcount;

	aatc_refcounted();
	virtual ~aatc_refcounted();

	void refcount_Add();
	void refcount_Release();
};

/*!\brief Basetype for script refcounted and GCd c++ objects to derive from.*/
class aatc_refcounted_GC{
public:
	asIScriptEngine* engine;
	mutable int refCount;
	mutable bool gcFlag;

	aatc_refcounted_GC(asIScriptEngine* engine);
	virtual ~aatc_refcounted_GC();

	void refcount_Add();
	void refcount_Release();

	int GetRefCount();
	void SetGCFlag();
	bool GetGCFlag();
	virtual void EnumReferences(asIScriptEngine* engine);
	virtual void ReleaseAllReferences(asIScriptEngine* engine);
};
//refcounted and gc basetype
//refcounted and gc basetype


bool aatc_templatecallback_container(asIObjectType *ot, bool &dontGarbageCollect);

//call before engine destruction, as engine is supposed to automatically call this
//because of SetEngineUserDataCleanupCallback(this function),
//but that didnt work during testing
void aatc_engine_cleanup(asIScriptEngine* engine);


void aect_iterator_template_generic_constructor_dummydefault(asIObjectType* objtype, void *memory);






class aatc_containerfunctor_Settings{
public:
	bool handlemode;
	bool handlemode_directcomp;
	asIScriptFunction* func_cmp;
	asIScriptFunction* func_equals;
	asIScriptFunction* func_hash;
};
/*!\brief Internal functor for container's comparisons.*/
class aatc_containerfunctor_comp : public aatc_containerfunctor_Settings{
public:
	asIScriptEngine* engine;
	aatc_engine_level_storage* els;
	aatc_containerfunctor_Settings* host_settings;

	bool need_init;
	aatc_containerfunctor_comp(asIScriptEngine* engine, aatc_containerfunctor_Settings* settings);

	bool operator()(const void* lhs, const void* rhs) const;
};
/*!\brief Internal functor for container's comparisons.*/
class aatc_containerfunctor_equals : public aatc_containerfunctor_Settings{
public:
	asIScriptEngine* engine;
	aatc_engine_level_storage* els;
	aatc_containerfunctor_Settings* host_settings;

	bool need_init;
	aatc_containerfunctor_equals(asIScriptEngine* engine, aatc_containerfunctor_Settings* settings);

	bool operator()(const void* lhs, const void* rhs) const;
};
/*!\brief Internal functor for container's comparisons.*/
class aatc_containerfunctor_hash : public aatc_containerfunctor_Settings{
public:
	asIScriptEngine* engine;
	aatc_engine_level_storage* els;
	aatc_containerfunctor_Settings* host_settings;

	bool need_init;
	aatc_containerfunctor_hash(asIScriptEngine* engine, aatc_containerfunctor_Settings* settings);

	aatc_hash_type operator()(const void* ptr) const;
};



/*!\brief Dummy class. Only the name is used for template black majicks*/
class aatc_Y{};
/*!\brief Dummy class. Only the name is used for template black majicks*/
class aatc_N{};
/*!\brief Dummy class. Only the name is used for template black majicks*/
class aatc_NULLCLASS{};



/*!\brief Used to pass function names from script to host. See manual page \ref page_manual_usage_si_funcpointer.*/
class aatc_script_Funcpointer : public aatc_refcounted{
public:
	asIScriptEngine* engine;
	//asIScriptContext* ctx;

	asIScriptFunction* func;
	aatc_type_string funcname;

	bool is_thiscall;
	asIScriptObject* so;

	bool ready;

	aatc_script_Funcpointer();
	~aatc_script_Funcpointer();

	static aatc_script_Funcpointer* Factory();

	bool Set(aatc_type_string funcname);//global function
	bool Set(aatc_type_string funcname, void* ref, int tid);//class method
	void Clear();

	void Prepare(asIScriptContext* context);
	void Execute(asIScriptContext* context);

	void scriptsidecall_CallVoid();

	//void ReleaseRef();
};

enum class aatc_DATAHANDLINGTYPE : int_fast8_t{
	PRIMITIVE,
	STRING,
	OBJECT,
	HANDLE
};
enum class aatc_PRIMITIVE_TYPE : int_fast8_t{
	INT8,
	INT16,
	INT32,
	INT64,
	UINT8,
	UINT16,
	UINT32,
	UINT64,
	FLOAT32,
	FLOAT64
};

aatc_DATAHANDLINGTYPE aatc_Determine_Datahandlingtype(aatc_type_uint32 astypeid);
aatc_PRIMITIVE_TYPE aatc_Determine_Primitivetype(aatc_type_uint32 astypeid);

/*!\brief This monstrosity is used to store any primitive or handle using the same allocation code.*/
struct aatc_primunion{
	union{
		aatc_type_int8 i8;
		aatc_type_int16 i16;
		aatc_type_int32 i32;
		aatc_type_int64 i64;

		aatc_type_uint8 ui8;
		aatc_type_uint16 ui16;
		aatc_type_uint32 ui32;
		aatc_type_uint64 ui64;

		aatc_type_float32 f32;
		aatc_type_float64 f64;

		void* ptr;
	};
};
extern aatc_primunion aatc_primunion_defaultvalue;

//for maps
class aatc_containerfunctor_map_Settings{
public:
	bool handlemode_directcomp;
	asIScriptFunction* func_cmp;
	asIScriptFunction* func_equals;
	asIScriptFunction* func_hash;

	aatc_DATAHANDLINGTYPE datahandlingid_key;
	aatc_PRIMITIVE_TYPE primitiveid_key;
};
/*!\brief Internal functor for container's comparisons.*/
class aatc_containerfunctor_map_comp : public aatc_containerfunctor_map_Settings{
public:
	asIScriptEngine* engine;
	aatc_engine_level_storage* els;
	aatc_containerfunctor_map_Settings* host_settings;

	bool need_init;
	aatc_containerfunctor_map_comp(asIScriptEngine* engine, aatc_containerfunctor_map_Settings* settings);

	bool operator()(const aatc_primunion& lhs, const aatc_primunion& rhs) const;
};
/*!\brief Internal functor for container's comparisons.*/
class aatc_containerfunctor_map_equals : public aatc_containerfunctor_map_Settings{
public:
	asIScriptEngine* engine;
	aatc_engine_level_storage* els;
	aatc_containerfunctor_map_Settings* host_settings;

	bool need_init;
	aatc_containerfunctor_map_equals(asIScriptEngine* engine, aatc_containerfunctor_map_Settings* settings);

	bool operator()(const aatc_primunion& lhs, const aatc_primunion& rhs) const;
};
/*!\brief Internal functor for container's comparisons.*/
class aatc_containerfunctor_map_hash : public aatc_containerfunctor_map_Settings{
public:
	asIScriptEngine* engine;
	aatc_engine_level_storage* els;
	aatc_containerfunctor_map_Settings* host_settings;

	bool need_init;
	aatc_containerfunctor_map_hash(asIScriptEngine* engine, aatc_containerfunctor_map_Settings* settings);

	aatc_hash_type operator()(const aatc_primunion& a) const;
};

/*!\brief Hash function for string. Apparently its fast and good.*/
aatc_hash_type aatc_hashfunc_djb2(const aatc_type_string& a);

/*
	errorchecking macros
*/
#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
#define aatc_errorcheck_container_missingfunctions_operation_retvoid(ID_OP,n_container,n_content,n_operation)	\
if(missing_functions & ID_OP){																					\
	aatc_errorprint_container_missingfunctions_operation_missing(n_container, n_content, n_operation);			\
	return;																										\
}

#define aatc_errorcheck_container_missingfunctions_operation_retnull(ID_OP,n_container,n_content,n_operation)	\
if(missing_functions & ID_OP){																					\
	aatc_errorprint_container_missingfunctions_operation_missing(n_container, n_content, n_operation);			\
	return NULL;																								\
}

#define aatc_errorcheck_container_missingfunctions_operation_noret(ID_OP,n_container,n_content,n_operation)	 	\
if(missing_functions & ID_OP){																					\
	aatc_errorprint_container_missingfunctions_operation_missing(n_container, n_content, n_operation);

#define aatc_errorcheck_container_access_empty_retvoid(n_container,n_content,n_operation)						\
if(T_container::empty()){																						\
	aatc_errorprint_container_access_empty(n_container, n_content, n_operation);								\
	return;																										\
}
#define aatc_errorcheck_container_access_empty_retnull(n_container,n_content,n_operation)						\
if(T_container::empty()){																						\
	aatc_errorprint_container_access_empty(n_container, n_content, n_operation);								\
	return NULL;																								\
}
#define aatc_errorcheck_container_access_empty_retdefault(n_container,n_content,n_operation)					\
if(T_container::empty()){																						\
	aatc_errorprint_container_access_empty(n_container, n_content, n_operation);								\
	return defaultvalue;																						\
}
#define aatc_errorcheck_container_access_bounds_retdefault(index,size,n_container,n_content,n_operation)		\
if((index>size)){																	\
	aatc_errorprint_container_access_bounds(aatc_type_sizetype(index), aatc_type_sizetype(size), n_container, n_content, n_operation);					\
	return defaultvalue;																						\
}
#else
#define aatc_errorcheck_container_missingfunctions_operation_retvoid(ID_OP,n_container,n_content,n_operation) {}
#define aatc_errorcheck_container_missingfunctions_operation_retnull(ID_OP,n_container,n_content,n_operation) {}
#define aatc_errorcheck_container_missingfunctions_operation_noret(ID_OP,n_container,n_content,n_operation) if(0){
#define aatc_errorcheck_container_access_empty_retvoid(n_container,n_content,n_operation) {}
#define aatc_errorcheck_container_access_empty_retnull(n_container,n_content,n_operation) {}
#define aatc_errorcheck_container_access_empty_retdefaultn_container,n_content,n_operation) {}
#define aatc_errorcheck_container_access_bounds_retdefault(index,size,n_container,n_content,n_operation) {}
#endif
	

/*!\brief Internal error printing function.*/
void aatc_errorprint_container_missingfunctions_operation_missing(const char* name_container, const char* name_content, const char* name_operation);
/*!\brief Internal error printing function.*/
void aatc_errorprint_container_access_empty(const char* name_container, const char* name_content, const char* name_operation);
/*!\brief Internal error printing function.*/
void aatc_errorprint_container_access_bounds(aatc_type_sizetype index, aatc_type_sizetype size, const char* name_container, const char* name_content, const char* name_operation);

//check for missing functions, return bitmask of missing functions
aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions_base(int CONTAINER_ID, aatc_template_specific_storage* tss);



template<int containerid> void aatc_register_container(asIScriptEngine* engine, aatc_Initializer* initializer){}
template<> void aatc_register_container<aatc_CONTAINERTYPE::VECTOR>(asIScriptEngine* engine, aatc_Initializer* initializer);
template<> void aatc_register_container<aatc_CONTAINERTYPE::LIST>(asIScriptEngine* engine, aatc_Initializer* initializer);
template<> void aatc_register_container<aatc_CONTAINERTYPE::SET>(asIScriptEngine* engine, aatc_Initializer* initializer);
template<> void aatc_register_container<aatc_CONTAINERTYPE::UNORDERED_SET>(asIScriptEngine* engine, aatc_Initializer* initializer);
template<> void aatc_register_container<aatc_CONTAINERTYPE::MAP>(asIScriptEngine* engine, aatc_Initializer* initializer);
template<> void aatc_register_container<aatc_CONTAINERTYPE::UNORDERED_MAP>(asIScriptEngine* engine, aatc_Initializer* initializer);

template<int CONTAINER_ID> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions(aatc_template_specific_storage* tss){ return 0; }
template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::VECTOR>(aatc_template_specific_storage* tss);
template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::LIST>(aatc_template_specific_storage* tss);
template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::SET>(aatc_template_specific_storage* tss);
template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::UNORDERED_SET>(aatc_template_specific_storage* tss);
template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::MAP>(aatc_template_specific_storage* tss);
template<> aatc_container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions<aatc_CONTAINERTYPE::UNORDERED_MAP>(aatc_template_specific_storage* tss);



END_AS_NAMESPACE
#endif