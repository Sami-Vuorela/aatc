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


#ifndef _includedh_aatc_container_listing
#define _includedh_aatc_container_listing



#include "aatc_common.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace listing {



			enum CONTAINER {
				VECTOR,
				LIST,
				SET,
				UNORDERED_SET,
				MAP,
				UNORDERED_MAP,

				_COUNT
			};



			template<int CONTAINER_ID> void register_container(asIScriptEngine* engine) {}
			template<int CONTAINER_ID> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template(enginestorage::template_specific_storage* tss) { return 0; }



			template<> void register_container<CONTAINER::VECTOR>(asIScriptEngine* engine);
			template<> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template<CONTAINER::VECTOR>(enginestorage::template_specific_storage* tss);

			//template<> void register_container<CONTAINER::LIST>(asIScriptEngine* engine);
			//template<> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template<CONTAINER::LIST>(enginestorage::template_specific_storage* tss);

			//template<> void register_container<CONTAINER::SET>(asIScriptEngine* engine);
			//template<> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template<CONTAINER::SET>(enginestorage::template_specific_storage* tss);

			//template<> void register_container<CONTAINER::UNORDERED_SET>(asIScriptEngine* engine);
			//template<> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template<CONTAINER::UNORDERED_SET>(enginestorage::template_specific_storage* tss);

			//template<> void register_container<CONTAINER::MAP>(asIScriptEngine* engine);
			//template<> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template<CONTAINER::MAP>(enginestorage::template_specific_storage* tss);

			//template<> void register_container<CONTAINER::UNORDERED_MAP>(asIScriptEngine* engine);
			//template<> common::container_operations_bitmask_type errorcheck_missing_functions_make_bitfield_for_template<CONTAINER::UNORDERED_MAP>(enginestorage::template_specific_storage* tss);



		};//namespace listing
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif