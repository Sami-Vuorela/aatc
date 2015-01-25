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
	
	{
		vector<int> cont;
		
		for(int i=0; i<10; i++){
			cont.push_back(i);
		}
		
		{
			auto it = cont.find_iterator(5);
			
			//cont.erase(it);
			//cont.erase(it);
			
			it++;
			it++;
			
			cont.erase(it);
		}
		
		
		
		for(auto it = cont.begin(); it++;){
			Print("val = " + it.value);
			
			//if(it.value == 5){
			//	cont.push_back(123);
			//}
		}
		
		
	}
	{
		Print("--------------------");
		vector<Material> cont;
		
		for(int i=0; i<10; i++){
			cont.push_back(Material(""+i,i));
		}
		
		{
			
			auto it = cont.find_iterator(Material(""+5,5));
			
			cont.erase(it);
			//cont.erase(it);
		}
		//cont.erase(Material(""+5,5));
		
		for(auto it = cont.begin(); it++;){
			Print("val = " + it.value.name);
			
			//if(it.value.id == 5){
			//	cont.push_back(Material("destroyerrr",0));
			//}
		}
		
		
		
		
		
		
	}
	
	Print("");
	Print("");
	Print("Tests successful = "+test_success_count);
	Print("Tests failed = "+test_failure_count);
}