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
// #include "00 auto iterator tests.as"
// #include "00 serializer tests.as"

void scriptmain(){
	TestContainer_vector();
	TestContainer_list();
	TestContainer_set();
	TestContainer_unordered_set();
	TestContainer_map();
	TestContainer_unordered_map();
	
	// test_auto_iterators();
	
	{//test container::end() and other iterator things
		Print("test container::end()");
		
		map<int,int> cont;
		
		cont[1] = 11;
		cont[3] = 33;
		cont[2] = 22;
		
		auto it = cont.begin();
		auto it_end = cont.end();
		
		Print("it IsEnd() = "+it.IsEnd());
		Print("it equals end = "+(it == it_end));
		Print("it equals end = "+(it == cont.end()));
		
		for(;it++;){
			Print("value = "+it.value);
		}
		
		Print("it IsEnd() = "+it.IsEnd());
		Print("it equals end = "+(it == it_end));
		Print("it equals end = "+(it == cont.end()));
		
		
	}
	
	Print("");
	Print("");
	Print("Tests successful = "+test_success_count);
	Print("Tests failed = "+test_failure_count);
}