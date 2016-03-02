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
//#include "00 auto iterator tests.as"
#include "00 serializer tests.as"


void scriptmain(){
	TestContainer_vector();
	TestContainer_list();
	TestContainer_set();
	TestContainer_unordered_set();
	TestContainer_map();
	TestContainer_unordered_map();
	
	
	{
	
		vector<int> cont;
		
		
		
		for(int i=10; i>0; i--){
			cont.push_back(i);
		}
		
		
		
		for(int i=10; i>0; i--){
			cont.push_back(i);
		}
		
		cont.sort_scriptfunc(function(l,r){
			if(l < r){
				return -1;
			}
			if(l > r){
				return 1;
			}
			return 0;
		});
		
		
		
		Print("------------");
		for(auto it = cont.begin();it++;){
			Print("it.value = " + it.value);
		}
	}
	{
	
		vector<Material> cont;
		
		for(int i=10; i>0; i--){
			cont.push_back(Material("" + i + "s",i));
		}
		
		// cont.sort_scriptfunc(function(l,r){
			// if(l.id < r.id){
				// return -1;
			// }
			// if(l.id > r.id){
				// return 1;
			// }
			// return 0;
		// });
		
		
		
		Print("------------");
		for(auto it = cont.begin();it++;){
			Print("it.value.id = " + it.value.id);
		}
	}
	
	
	Print("");
	Print("");
	Print("Tests successful = "+test_success_count);
	Print("Tests failed = "+test_failure_count);
}
