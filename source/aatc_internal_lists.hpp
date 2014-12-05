#ifndef _includedh_aatc_internal_lists
#define _includedh_aatc_internal_lists

#include <tuple>
#include "aatc_config.hpp"



class aatc_info_container_vector{
public:
	static const int container_id = aatc_CONTAINERTYPE::VECTOR;
	static const bool is_associative = 0;
	static const bool is_hashed = 0;
};
class aatc_info_container_list{
public:
	static const int container_id = aatc_CONTAINERTYPE::LIST;
	static const bool is_associative = 0;
	static const bool is_hashed = 0;
};
class aatc_info_container_set{
public:
	static const int container_id = aatc_CONTAINERTYPE::SET;
	static const bool is_associative = 1;
	static const bool is_hashed = 0;
};
class aatc_info_container_unordered_set{
public:
	static const int container_id = aatc_CONTAINERTYPE::UNORDERED_SET;
	static const bool is_associative = 1;
	static const bool is_hashed = 1;
};
class aatc_info_container_map{
public:
	static const int container_id = aatc_CONTAINERTYPE::MAP;
	static const bool is_associative = 1;
	static const bool is_hashed = 0;
};
class aatc_info_container_unordered_map{
public:
	static const int container_id = aatc_CONTAINERTYPE::UNORDERED_MAP;
	static const bool is_associative = 1;
	static const bool is_hashed = 1;
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



#endif