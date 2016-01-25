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

#include "aatc_config.hpp"



BEGIN_AS_NAMESPACE

#if aatc_CONFIG_USE_ASADDON_SERIALIZER
	class CSerializer;
	class CSerializedValue;
#endif

namespace aatc {

	namespace container {
		namespace shared {
			class container_basicbase;
		};
	};

	namespace enginestorage {
		class template_specific_storage;
		class containertype_specific_storage;
		class engine_level_storage;
	};//namespace enginestorage

	#if aatc_CONFIG_USE_ASADDON_SERIALIZER
		namespace serializer {
			typedef bool(*funcptr_t_container_basicbase_is_thistype)(container::shared::container_basicbase* base);
			typedef void(*funcptr_t_container_basicbase_process)(container::shared::container_basicbase* base, CSerializedValue* val);
		};//namespace serializer
	#endif

	namespace common {



		/*!\brief Minimal spinlock class*/
		class std_Spinlock {
		public:
			//std::atomic<int> m_state = ATOMIC_VAR_INIT(1);//this thing useful?
			std::atomic<int> state;
			std_Spinlock();
			void lock();
			void unlock();
		};

		//doxygen skip
		#ifndef DOXYGEN_SHOULD_SKIP_THIS

			//this beautiful creation makes std::list<int>::sort cry salty compiler tears in debug mode for some reason
			template<typename T, typename T_other_functor> class aatc_functor_notcmp{
			public:
				bool operator()(const T& lhs, const T& rhs) const{
					return !(T_other_functor()(lhs, rhs));
				}
			};
			template<typename T, typename T_other_functor> class aatc_functor_notcmp_persistent_noconst : public T_other_functor{
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



		class iterator_base{
		public:
			bool firstt;
			bool cont;

			#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
				int_fast16_t safety_iteratorversion;
			#endif

			iterator_base();
			iterator_base(const iterator_base& other);
		};

		/*
			Enum listing the different container operations
			that require script functions to be called.
			Used mostly for error checking.
		*/
		enum CONTAINER_OPERATION{
			COUNT = 1,
			ERASE_VALUE = 2,
			INSERT = 4,
			SORT = 8,
			CONTAINS = 16,
			REMOVE = 32,
			FIND = 64
		};

		typedef config::t::uint32 container_operations_bitmask_type;

		//doxygen skip
		#ifndef DOXYGEN_SHOULD_SKIP_THIS

		namespace reghelp {
			template<class T> void constructor(void *memory) { new(memory)T(); }
			template<class T, class T_other> void constructor_copy(const T_other& other, void *memory) { new(memory)T(other); }
			template<class T, class T_input1> void constructor_1_param(T_input1 x, void *memory) { new(memory)T(x); }
			template<typename T_out, typename T_host> T_out construct_hosted(T_host cont) {
				return T_out(cont);
			}
			template<class T> void constructor_template_default(asIObjectType* objtype, void *memory) { new(memory)T(); }

			template<class T> void generic_destructor(void *memory) {
				((T*)memory)->~T();
			}
		};

		#endif
		//doxygen skip



		//refcounted and gc basetype
		//refcounted and gc basetype

		/*!\brief Basetype for script refcounted c++ objects to derive from.*/
		class aatc_refcounted{
		public:
			mutable int refcount;

			aatc_refcounted();
			virtual ~aatc_refcounted();

			void refcount_Add();
			void refcount_Release();
		};

		/*!\brief Basetype for script refcounted and GCd c++ objects to derive from.*/
		class aatc_refcounted_GC{
		public:
			mutable int refCount;
			mutable bool gcFlag;

			aatc_refcounted_GC();
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


		bool aatc_templatecallback_1tp(asIObjectType *ot, bool &dontGarbageCollect);
		bool aatc_templatecallback_map(asIObjectType *ot, bool &dontGarbageCollect);
		bool aatc_templatecallback_typeid(asIObjectType *ot, int typeId, bool &dontGarbageCollect);

		//call before engine destruction, as engine is supposed to automatically call this
		//because of SetEngineUserDataCleanupCallback(this function),
		//but that didnt work during testing
		void aatc_engine_cleanup(asIScriptEngine* engine);


		void aect_iterator_template_generic_constructor_dummydefault(asIObjectType* objtype, void *memory);


		class RegistrationState {
		public:
			const static int bufsize = 10000;

			asIScriptEngine* engine;

			char n_container[bufsize];
			char n_container_T[bufsize];
			char n_container_class_T[bufsize];

			char n_content[bufsize];
			char n_containerTcontentT[bufsize];
			
			char n_iterator[bufsize];
			char n_iterator_T[bufsize];
			char n_iterator_class_T[bufsize];
			
			char textbuf[bufsize];
			int error;

			RegistrationState(asIScriptEngine* engine);
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
			config::t::string funcname;

			bool is_thiscall;
			asIScriptObject* so;

			bool ready;

			aatc_script_Funcpointer();
			~aatc_script_Funcpointer();

			static aatc_script_Funcpointer* Factory();

			bool Set(config::t::string funcname);//global function
			bool Set(config::t::string funcname, void* ref, int tid);//class method
			void Clear();

			void Prepare(asIScriptContext* context);
			void Execute(asIScriptContext* context);

			void scriptsidecall_CallVoid();

			//void ReleaseRef();
		};

		enum class DATAHANDLINGTYPE : int_fast8_t{
			PRIMITIVE,
			STRING,
			OBJECT,
			HANDLE
		};
		enum class PRIMITIVE_TYPE : int_fast8_t{
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

		DATAHANDLINGTYPE aatc_Determine_Datahandlingtype(asIScriptEngine* engine,config::t::uint32 astypeid);
		PRIMITIVE_TYPE aatc_Determine_Primitivetype(config::t::uint32 astypeid);

		/*!\brief This monstrosity is used to store any primitive or handle using the same allocation code.*/
		struct primunion{
			union{
				config::t::int8 i8;
				config::t::int16 i16;
				config::t::int32 i32;
				config::t::int64 i64;

				config::t::uint8 ui8;
				config::t::uint16 ui16;
				config::t::uint32 ui32;
				config::t::uint64 ui64;

				config::t::float32 f32;
				config::t::float64 f64;

				void* ptr;
			};
			void* Get_Ptr_To_Primitive_Type(PRIMITIVE_TYPE primtype);
			const void* Get_Ptr_To_Primitive_Type_const(PRIMITIVE_TYPE primtype)const;

			void From_voidptr_to_primitive(void* target, PRIMITIVE_TYPE primtype);

			void Init();
		};
		extern primunion primunion_defaultvalue;
		typedef std::pair<primunion, primunion> primunion_pair;



		/*
			errorchecking macros
		*/
		#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME
		#define aatc_errorcheck_container_missingfunctions_operation_retvoid(ID_OP,n_container,n_content,n_operation)	\
		if(missing_functions & ID_OP){																					\
			common::errorprint::container::missingfunctions_operation_missing(n_container, n_content, n_operation);			\
			return;																										\
		}

		#define aatc_errorcheck_container_missingfunctions_operation_retnull(ID_OP,n_container,n_content,n_operation)	\
		if(missing_functions & ID_OP){																					\
			common::errorprint::container::missingfunctions_operation_missing(n_container, n_content, n_operation);			\
			return NULL;																								\
		}

		#define aatc_errorcheck_container_missingfunctions_operation_noret(ID_OP,n_container,n_content,n_operation)	 	\
		if(missing_functions & ID_OP){																					\
			common::errorprint::container::missingfunctions_operation_missing(n_container, n_content, n_operation);

		#define aatc_errorcheck_container_access_empty_retvoid(n_container,n_content,n_operation)						\
		if(T_container::empty()){																						\
			common::errorprint::container::access_empty(n_container, n_content, n_operation);								\
			return;																										\
		}
		#define aatc_errorcheck_container_access_empty_retnull(n_container,n_content,n_operation)						\
		if(t->container.empty()){																						\
			aatc::common::errorprint::container::access_empty(n_container, n_content, n_operation);								\
			return NULL;																								\
		}
		#define aatc_errorcheck_container_access_empty_retdefault(n_container,n_content,n_operation)					\
		if(t->container.empty()){																						\
			aatc::common::errorprint::container::access_empty(n_container, n_content, n_operation);								\
			return defaultvalue;																						\
		}
		#define aatc_errorcheck_container_access_bounds_retdefault(index,size,n_container,n_content,n_operation)		\
		if((index>size)){																	\
			common::errorprint::container::access_bounds(config::t::sizetype(index), config::t::sizetype(size), n_container, n_content, n_operation);					\
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

		#if aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS
			#define aatc_errorcheck_container_iterator_safety_version_Increment() iterator_safety_version++;
		#else
			#define aatc_errorcheck_container_iterator_safety_version_Increment()
		#endif




		namespace errorprint {
			namespace container {
				void missingfunctions_operation_missing(const char* name_container, const char* name_content, const char* name_operation);
				void access_empty(const char* name_container, const char* name_content, const char* name_operation);
				void access_bounds(config::t::sizetype index, config::t::sizetype size, const char* name_container, const char* name_content, const char* name_operation);
				void iterator_invalid();
			};
			namespace iterator {
				void container_modified();
			};
		};

		///*!\brief Internal error printing function.*/
		//void common::errorprint::container::missingfunctions_operation_missing(const char* name_container, const char* name_content, const char* name_operation);
		///*!\brief Internal error printing function.*/
		//void common::errorprint::container::access_empty(const char* name_container, const char* name_content, const char* name_operation);
		///*!\brief Internal error printing function.*/
		//void common::errorprint::container::access_bounds(config::t::sizetype index, config::t::sizetype size, const char* name_container, const char* name_content, const char* name_operation);
		///*!\brief Internal error printing function.*/
		//void aatc_errorprint_iterator_container_modified();
		///*!\brief Internal error printing function.*/
		//void common::errorprint::container::iterator_invalid();

		//check for missing functions, return bitmask of missing functions
		//container_operations_bitmask_type aatc_errorcheck_container_type_missing_functions_base(int CONTAINER_ID, enginestorage::template_specific_storage* tss);



	};//namespace common
};//namespace aatc
END_AS_NAMESPACE



#endif