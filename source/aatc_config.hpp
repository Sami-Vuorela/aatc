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



#ifndef _includedh_aatc_config
#define _includedh_aatc_config


//#include "cm/core/thirdparty/tp_angelscript.hpp"
#include <stdio.h>
#include <stdint.h>
#include "angelscript.h"




/*
	BOOST POWERR!!
	Will use boost containers instead of std containers when available.
*/
#define aatc_CONFIG_USE_BOOST 0


/*
	Enable this if you want the containers to have a
	template specialization of the official addon ref object.
	You must provide a path to the addon's header.
	You must have modified the ref addon to make
	the members m_ref and m_type public.
*/
#define aatc_CONFIG_USE_ASADDON_REF 0
#define aatc_CONFIG_USE_ASADDON_REF_PATH "C:\Dev\VC2013\angelscript_2.28.2\add_on\scripthandle\scripthandle.h"

/*
	Directly compare handles as pointers in c++ instead of comparing the script objects by calling script functions.
	Blazing c++ speed but no script flexibility. Good for "stupid" pools of handles.
	This option is but a default, all container objects can have this set individually.
*/
#define aatc_CONFIG_DEFAULT_HANDLEMODE_DIRECTCOMP 0

/*
	Check for missing required methods (opEquals,opCmp,hash) in runtime, takes one bitwise-and operation
	per angelscript function call to a function that requires script functions, not much but something.
	With this enabled, missing methods will cause nothing to happen and raise an exception (if exceptions are enabled),
	without this missing methods will probably crash.
*/
#define aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME 1
#define aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_EXCEPTIONS 1

/*
	If any methods are missing, raise an exception during container constructor.
	Good for error checking with minimal runtime performance hit.
	Some methods might be missing but never called, in those cases this will be nothing but trouble.
*/
#define aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE 0
/*
	If missing methods are detected and the container's subtype is a handle,
	force the container into directcomp mode where script methods are not needed instead of raising an exception.
*/
#define aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE_USE_DIRECTCOMP 1

/*
	Random magical optimization numbers ahead.
*/
//this number was used by boost, so it must be legit
#define aatc_CONFIG_DEFAULT_CONTAINER_UNORDERED_SET_DEFAULTBUCKETCOUNT 11
#define aatc_CONFIG_DEFAULT_CONTAINER_UNORDERED_MAP_DEFAULTBUCKETCOUNT 11
#define aatc_engine_userdata_id 8899



#define aatc_name_script_container_vector "vector"
#define aatc_name_script_container_list "list"
#define aatc_name_script_container_set "set"
#define aatc_name_script_container_unordered_set "unordered_set"
#define aatc_name_script_container_map "map"
#define aatc_name_script_container_unordered_map "unordered_map"

#define aatc_name_script_container_method_set_directcomp "SetDirectcomp"
#define aatc_name_script_container_method_clear "clear"
#define aatc_name_script_container_method_size "size"
#define aatc_name_script_container_method_count "count"
#define aatc_name_script_container_method_empty "empty"
#define aatc_name_script_container_method_swap "swap"
#define aatc_name_script_container_method_front "front"
#define aatc_name_script_container_method_back "back"
#define aatc_name_script_container_method_push_front "push_front"
#define aatc_name_script_container_method_push_back "push_back"
#define aatc_name_script_container_method_pop_front "pop_front"
#define aatc_name_script_container_method_pop_back "pop_back"
#define aatc_name_script_container_method_reserve "reserve"
#define aatc_name_script_container_method_insert "insert"
#define aatc_name_script_container_method_erase "erase"
#define aatc_name_script_container_method_sort "sort"
#define aatc_name_script_container_method_contains "contains"
#define aatc_name_script_container_method_find "find"
#define aatc_name_script_container_method_erase_index "erase_index"
#define aatc_name_script_container_method_erase_value "erase_value"
#define aatc_name_script_container_method_insert_index_before "insert_index_before"
#define aatc_name_script_container_method_insert_index_after "insert_index_after"
#define aatc_name_script_container_method_begin "begin"

#define aatc_name_script_iterator "_iterator"
#define aatc_name_script_funcpointer "aatc_funcpointer"
#define aatc_name_script_requiredmethod_hash "hash"

//primitive typedefs, set these appropriately for your platform if stdint doesnt work
typedef uint8_t		aatc_type_uint8;
typedef uint16_t	aatc_type_uint16;
typedef uint32_t	aatc_type_uint32;
typedef uint64_t	aatc_type_uint64;
typedef int8_t		aatc_type_int8;
typedef int16_t		aatc_type_int16;
typedef int32_t		aatc_type_int32;
typedef int64_t		aatc_type_int64;
typedef float		aatc_type_float32;
typedef double		aatc_type_float64;

#include <string>
typedef std::string			aatc_type_string;//use whatever you use in script (users of angelscript addon scriptstdstring should use std::string here)

typedef aatc_type_int32 aatc_type_sizetype;
#define aatc_name_script_sizetype "int"

/*
	edit: this doesnt apply to map and uo map, they always support primitives
	You can tick off primitive tempspecs here if you know you will never use them.
	Primitives are currently not supported at all without template specializations,
	so if you try to use them in script with the primitive ticked off here,
	the generic type container which is supposed to be used for everything but primitives
	will be told to use primitives and it will explode horribly.
*/
#define aatc_include_primitive_native_tempspec_INT8		1
#define aatc_include_primitive_native_tempspec_INT16	1
#define aatc_include_primitive_native_tempspec_INT32	1
#define aatc_include_primitive_native_tempspec_INT64	1
#define aatc_include_primitive_native_tempspec_UINT8	1
#define aatc_include_primitive_native_tempspec_UINT16	1
#define aatc_include_primitive_native_tempspec_UINT32	1
#define aatc_include_primitive_native_tempspec_UINT64	1
#define aatc_include_primitive_native_tempspec_FLOAT32	1
#define aatc_include_primitive_native_tempspec_FLOAT64	1
#define aatc_include_primitive_native_tempspec_STRING	1
#define aatc_include_primitive_native_tempspec_REF		1

#if aatc_CONFIG_USE_BOOST
	#include <vector>
	#include <list>
	#include <set>
	#include <boost/unordered_set.hpp>
	#include <map>
	#include <boost/unordered_map.hpp>
	#include <boost/thread/thread.hpp>
	#include <boost/thread/mutex.hpp>
#else
	#include <vector>
	#include <list>
	#include <set>
	#include <unordered_set>
	#include <map>
	#include <unordered_map>
	#include <mutex>
	#include <algorithm>
	#include <atomic>
#endif

#if aatc_CONFIG_USE_ASADDON_REF
	#include aatc_CONFIG_USE_ASADDON_REF_PATH
#endif

/*
	Actual container classes to use.
	ait  = actual implementation type
	acit = actual container implementation type
*/

#if aatc_CONFIG_USE_BOOST
	#define aatc_ait_storage_map boost::unordered_map
	#define aatc_ait_storage_pair std::pair
	#define aatc_ait_fastlock boost::detail::spinlock
	#define aatc_acit_vector std::vector
	#define aatc_acit_list std::list
	#define aatc_acit_set std::set
	#define aatc_acit_unordered_set boost::unordered_set
	#define aatc_acit_map std::map
	#define aatc_acit_unordered_map boost::unordered_map
	#define aatc_acit_flatset std::set//boost::flatset didnt work at the time, might work for you just fine
#else
	#define aatc_ait_storage_map std::unordered_map
	#define aatc_ait_storage_pair std::pair
	#define aatc_ait_fastlock aatc_std_Spinlock
	#define aatc_acit_vector std::vector
	#define aatc_acit_list std::list
	#define aatc_acit_set std::set
	#define aatc_acit_unordered_set std::unordered_set
	#define aatc_acit_map std::map
	#define aatc_acit_unordered_map std::unordered_map
	#define aatc_acit_flatset std::set
#endif





/*
	Enum listing the different containers,
	used for template specailizations and other magics.
*/
enum aatc_CONTAINERTYPE{
	VECTOR,
	LIST,
	MAP,
	UNORDERED_MAP,
	SET,
	UNORDERED_SET,
	FLATSET,
	QUEUE,
	DEQUEUE,
	STACK,
	aatc_CONTAINERTYPE_COUNT
};



/*
	Pimp your error messages here.
*/
#define aatc_errormessage_funcpointer_nothandle "Type '%s' not input as a handle."
#define aatc_errormessage_container_missingfunctions_formatting_param1 name_content
#define aatc_errormessage_container_missingfunctions_formatting_param2 name_container
#define aatc_errormessage_container_missingfunctions_formatting_param3 name_operation
#define aatc_errormessage_container_missingfunctions_formatting "Type '%s' has no method required for container's '%s::%s' method."
#define aatc_errormessage_container_access_empty_formatting_param1 name_container
#define aatc_errormessage_container_access_empty_formatting_param2 name_content
#define aatc_errormessage_container_access_empty_formatting_param3 name_operation
#define aatc_errormessage_container_access_empty_formatting "%s<%s>::%s called but the container is empty."
#define aatc_errormessage_container_access_bounds_formatting_param1 name_container
#define aatc_errormessage_container_access_bounds_formatting_param2 name_content
#define aatc_errormessage_container_access_bounds_formatting_param3 name_operation
#define aatc_errormessage_container_access_bounds_formatting_param4 index
#define aatc_errormessage_container_access_bounds_formatting_param5 size
#define aatc_errormessage_container_access_bounds_formatting "%s<%s>::%s[%i] is out of bounds. Size = %i."






#endif