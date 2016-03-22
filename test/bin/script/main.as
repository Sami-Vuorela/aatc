bool test_enable_print_test_start = false;//for your c++ crashing needs
bool test_enable_print_success = false;
bool test_enable_print_failure = true;

#include "basics.as"
#include "test_container_vector.as"
#include "test_container_list.as"
#include "test_container_deque.as"
#include "test_container_set.as"
#include "test_container_unordered_set.as"
#include "test_container_map.as"
#include "test_container_unordered_map.as"

#include "test_lambda.as"

#include "test_interop.as"

//#include "00 auto iterator tests.as"
// #include "00 serializer tests.as"



void scriptmain(){
	TestContainer_vector();
	TestContainer_list();
	TestContainer_deque();
	TestContainer_set();
	TestContainer_unordered_set();
	TestContainer_map();
	TestContainer_unordered_map();
	
	Test_lambda();
	
	
	
	Print("");
	Print("");
	Print("Tests successful = "+test_success_count);
	Print("Tests failed = "+test_failure_count);
}
