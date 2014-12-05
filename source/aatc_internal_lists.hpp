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


#ifndef _includedh_aatc_internal_lists
#define _includedh_aatc_internal_lists

#include <tuple>
#include "aatc_config.hpp"

#include "aatc_vector.hpp"
#include "aatc_list.hpp"
#include "aatc_set.hpp"
#include "aatc_unordered_set.hpp"
#include "aatc_map.hpp"
#include "aatc_unordered_map.hpp"


BEGIN_AS_NAMESPACE


class aatc_info_container_vector{
public:
	static const int container_id = aatc_CONTAINERTYPE::VECTOR;
	static const bool is_associative = 0;
	static const bool is_hashed = 0;
	typedef aatc_container_vector_template container_type_template;
};
class aatc_info_container_list{
public:
	static const int container_id = aatc_CONTAINERTYPE::LIST;
	static const bool is_associative = 0;
	static const bool is_hashed = 0;
	typedef aatc_container_list_template container_type_template;
};
class aatc_info_container_set{
public:
	static const int container_id = aatc_CONTAINERTYPE::SET;
	static const bool is_associative = 1;
	static const bool is_hashed = 0;
	typedef aatc_container_set_template container_type_template;
};
class aatc_info_container_unordered_set{
public:
	static const int container_id = aatc_CONTAINERTYPE::UNORDERED_SET;
	static const bool is_associative = 1;
	static const bool is_hashed = 1;
	typedef aatc_container_unordered_set_template container_type_template;
};
class aatc_info_container_map{
public:
	static const int container_id = aatc_CONTAINERTYPE::MAP;
	static const bool is_associative = 1;
	static const bool is_hashed = 0;
	typedef aatc_container_map_template container_type_template;
};
class aatc_info_container_unordered_map{
public:
	static const int container_id = aatc_CONTAINERTYPE::UNORDERED_MAP;
	static const bool is_associative = 1;
	static const bool is_hashed = 1;
	typedef aatc_container_unordered_map_template container_type_template;
};



typedef std::tuple<
	aatc_info_container_vector,
	aatc_info_container_list,
	aatc_info_container_set,
	aatc_info_container_unordered_set,
	aatc_info_container_map,
	aatc_info_container_unordered_map
> aatc_infos_all_tuple;
const int aatc_infos_all_tuple_size = std::tuple_size<aatc_infos_all_tuple>::value;

typedef std::tuple<
	aatc_info_container_vector,
	aatc_info_container_list,
	aatc_info_container_set,
	aatc_info_container_unordered_set
> aatc_infos_1tp_tuple;
const int aatc_infos_1tp_tuple_size = std::tuple_size<aatc_infos_1tp_tuple>::value;



END_AS_NAMESPACE

#endif