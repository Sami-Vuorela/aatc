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


void tester_1(vector<int>@ cont){
	Print("size = " + cont.size());
}
void tester_2(vector<int> &in cont){
	Print("size = " + cont.size());
}
void tester_3(vector<int> &out cont){
	cont.push_back(2);
}
void tester_4(vector<int> &inout cont){
	cont.push_back(2);
}

void scriptmain(){
	TestContainer_vector();
	TestContainer_list();
	TestContainer_deque();
	TestContainer_set();
	TestContainer_unordered_set();
	TestContainer_map();
	TestContainer_unordered_map();
	
	Test_lambda();
	
	{
		vector<int> cont;
		cont.push_back(1);
		
		tester_1(cont);
		
		
		vector<int> cont2(cont);
		
		// cont2 = cont;
		
		
		// Print("size = " + cont.size());
		// tester_2(cont);
		// Print("size = " + cont.size());
		// tester_3(cont);
		// Print("size = " + cont.size());
		tester_4(cont2);
		Print("size = " + cont2.size());
	}
	
	{
		deque<int> cont;
		
		for(int i=0; i<100; i++){
			cont.push_back(i);
		}
		
		for(auto it = cont.begin();it++;){
			Print("" + it.value);
		}
	}
	
	
	
	Print("");
	Print("");
	Print("Tests successful = "+test_success_count);
	Print("Tests failed = "+test_failure_count);
}
