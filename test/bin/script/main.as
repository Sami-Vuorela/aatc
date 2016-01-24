bool test_enable_print_test_start = false;//for your c++ crashing needs
bool test_enable_print_success = false;
bool test_enable_print_failure = true;

#include "basics.as"
//#include "test_container_vector.as"
//#include "test_container_list.as"
//#include "test_container_set.as"
//#include "test_container_unordered_set.as"
#include "test_container_map.as"
#include "test_container_unordered_map.as"
// #include "00 auto iterator tests.as"
// #include "00 serializer tests.as"


void scriptmain(){
	//TestContainer_vector();
	//TestContainer_list();
	//TestContainer_set();
	//TestContainer_unordered_set();
	TestContainer_map();
	TestContainer_unordered_map();
	
	{
		vector<Material> cont;
		
		cont.push_back(Material("1s",1));
		cont.push_back(Material("2s",2));
		
		for(auto it = cont.begin(); it++;){
			Print(it.value.name);
		}
		
		for(vector_iterator<Material> it(cont); it++;){
			Print(it.value.name);
		}
		
		Print(cont.back().name);
	}
	Print("");
	{
		vector<int> cont;
		
		cont.push_back(1);
		cont.push_back(2);
		cont.push_back(3);
		
		for(auto it = cont.begin(); it++;){
			Print(""+it.value);
		}
		
		for(vector_iterator<int> it(cont); it++;){
			Print(""+it.value);
		}
		
		Print(""+cont.back());
	}
	Print("");
	{
		vector<string> cont;
		
		cont.push_back("1.11s");
		cont.push_back("2.22s");
		cont.push_back("3.33s");
		
		for(auto it = cont.begin(); it++;){
			Print(""+it.value);
		}
		
		for(vector_iterator<string> it(cont); it++;){
			Print(""+it.value);
		}
		
		Print(""+cont.back());
	}
	Print("");
	{
		unordered_map<int,int> cont;
		
		cont.insert(1,111);
		cont.insert(3,333);
		cont.insert(2,222);
		
		for(auto it = cont.begin(); it++;){
			Print(""+ it.key +" , " + it.value);
		}
	}
	Print("");
	{
		unordered_map<Material,int> cont;
		
		cont.insert(Material("1s",1),111);
		cont.insert(Material("2s",2),222);
		cont.insert(Material("3s",3),333);
		
		cont.erase(Material("2s",2));
		
		for(auto it = cont.begin(); it++;){
			Print(""+ it.key.name +" , " + it.value);
		}
	}
	Print("");
	{
		map<int,int> cont;
		
		cont.insert(1,11);
		cont.insert(3,33);
		cont.insert(2,22);
		
		cont.erase(3);
		
		for(auto it = cont.begin(); it++;){
			Print(""+ it.key +" , " + it.value);
		}
	}
	Print("");
	{
		vector<float> cont;
		
		cont.push_back(1.11);
		cont.push_back(3.33);
		cont.push_back(2.22);
		
		cont.sort();
		
		cont.insert(3,999);
		
		{
			auto it = cont.find_iterator(3.33);
			cont.insert(it,9000);
		}
		
		for(auto it = cont.begin(); it++;){
			Print(""+it.value);
		}
		
		for(vector_iterator<float> it(cont); it++;){
			Print(""+it.value);
		}
		
		Print("");
		Print(""+cont[1]);
		
		cont.erase(2);
		
		Print("");
		Print(""+cont.back());
	}
	Print("");
	{
		vector<float> cont;
		
		for(int i=0; i<10; i++){
			cont.push_back(i);
		}
		
		cont.erase(3,6);
		
		for(auto it = cont.begin(); it++;){
			Print(""+it.value);
		}
	}
	
	Print("");
	Print("");
	Print("Tests successful = "+test_success_count);
	Print("Tests failed = "+test_failure_count);
}

void scriptmain2(vector<int>@ cont){
		for(auto it = cont.begin(); it++;){
			Print(""+it.value);
		}
		
		Print(""+cont.back());
		
		Print("");
		Print("");
		Print("");
}

void test_container_interface_map_1(map<int,int>@ cont){
		for(auto it = cont.begin(); it++;){
			Print(""+ it.key +" , " + it.value);
		}
		
		Print("");
		Print("");
		Print("");
}