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



#ifndef _includedh_aatc_container_switches
#define _includedh_aatc_container_switches

#include "aatc_common.hpp"


#define aatc_container_vector_needfunc_BACK_WRITE 1
#define aatc_container_vector_needfunc_BACK_READ 1
#define aatc_container_vector_needfunc_FRONT_WRITE 0
#define aatc_container_vector_needfunc_FRONT_READ 1
#define aatc_container_vector_needfunc_ERASE_POSITION 1
#define aatc_container_vector_needfunc_ERASE_VALUE 0
#define aatc_container_vector_needfunc_OP_INDEX 1
#define aatc_container_vector_needfunc_RESERVE 1
#define aatc_container_vector_needfunc_INSERT 0
#define aatc_container_vector_needfunc_SORT_NATIVE 0
#define aatc_container_vector_needfunc_SORT_GENERIC 1
#define aatc_container_vector_needfunc_CONTAINS_NATIVE 0
#define aatc_container_vector_needfunc_ERASE_GENERIC_INDEX aatc_Y
#define aatc_container_vector_needfunc_ERASE_GENERIC_VALUE 1
#define aatc_container_vector_needfunc_INSERT_GENERIC_INDEX aatc_Y


#define aatc_container_list_needfunc_BACK_WRITE 1
#define aatc_container_list_needfunc_BACK_READ 1
#define aatc_container_list_needfunc_FRONT_WRITE 1
#define aatc_container_list_needfunc_FRONT_READ 1
#define aatc_container_list_needfunc_ERASE_POSITION 0
#define aatc_container_list_needfunc_ERASE_VALUE 0
#define aatc_container_list_needfunc_OP_INDEX 0
#define aatc_container_list_needfunc_RESERVE 0
#define aatc_container_list_needfunc_INSERT 0
#define aatc_container_list_needfunc_SORT_NATIVE 1
#define aatc_container_list_needfunc_SORT_GENERIC 0
#define aatc_container_list_needfunc_CONTAINS_NATIVE 0
#define aatc_container_list_needfunc_ERASE_GENERIC_INDEX aatc_N
#define aatc_container_list_needfunc_ERASE_GENERIC_VALUE 1
#define aatc_container_list_needfunc_INSERT_GENERIC_INDEX aatc_N


#define aatc_container_set_needfunc_BACK_WRITE 0
#define aatc_container_set_needfunc_BACK_READ 0
#define aatc_container_set_needfunc_FRONT_WRITE 0
#define aatc_container_set_needfunc_FRONT_READ 0
#define aatc_container_set_needfunc_ERASE_POSITION 0
#define aatc_container_set_needfunc_ERASE_VALUE 1
#define aatc_container_set_needfunc_OP_INDEX 0
#define aatc_container_set_needfunc_RESERVE 0
#define aatc_container_set_needfunc_INSERT 1
#define aatc_container_set_needfunc_SORT_NATIVE 0
#define aatc_container_set_needfunc_SORT_GENERIC 0
#define aatc_container_set_needfunc_CONTAINS_NATIVE 1


#define aatc_container_unordered_set_needfunc_BACK_WRITE 0
#define aatc_container_unordered_set_needfunc_BACK_READ 0
#define aatc_container_unordered_set_needfunc_FRONT_WRITE 0
#define aatc_container_unordered_set_needfunc_FRONT_READ 0
#define aatc_container_unordered_set_needfunc_ERASE_POSITION 0
#define aatc_container_unordered_set_needfunc_ERASE_VALUE 1
#define aatc_container_unordered_set_needfunc_OP_INDEX 0
#define aatc_container_unordered_set_needfunc_RESERVE 0
#define aatc_container_unordered_set_needfunc_INSERT 1
#define aatc_container_unordered_set_needfunc_SORT_NATIVE 0
#define aatc_container_unordered_set_needfunc_SORT_GENERIC 0
#define aatc_container_unordered_set_needfunc_CONTAINS_NATIVE 1








#endif