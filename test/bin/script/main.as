bool test_enable_print_test_start = false;//for your c++ crashing needs
bool test_enable_print_success = false;
bool test_enable_print_failure = true;

#include "basics.as"
#include "test_container_vector.as"
#include "test_container_list.as"
#include "test_container_set.as"
#include "test_container_unordered_set.as"
#include "test_container_map.as"
#include "test_container_unordered_map.as"

void scriptmain(){
	TestContainer_vector();
	TestContainer_list();
	TestContainer_set();
	TestContainer_unordered_set();
	TestContainer_map();
	TestContainer_unordered_map();
	
	Print("");
	Print("");
	Print("Test successfull = "+test_success_count);
	Print("Test failed = "+test_failure_count);
}