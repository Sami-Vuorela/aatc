string test_name;
string test_fail_reason;
bool test_success;
int test_success_count = 0;
int test_failure_count = 0;

void test_Startnew(string &in name){
	test_success = false;
	test_name = name;
	if(test_enable_print_test_start){
		Print("test "+test_name);
	}
}
void test_PrintSuccess(){
	if(test_success){
		test_success_count++;
		if(test_enable_print_success){
			Print("test "+test_name);
			Print("  passed");
			Print("");
		}
	}else{
		test_failure_count++;
		if(test_enable_print_failure){
			Print("test "+test_name);
			Print("  failed");
			Print("    "+test_fail_reason);
			Print("");
		}
	}
}





//some classes for testing
int objectcount_material = 0;

class Material{
	string name;
	int id;
	Material(){
		objectcount_material++;
		// Print("object constructor   count="+objectcount_material);
	}
	Material(string _name,int _id){
		name = _name;
		id = _id;
		objectcount_material++;
		 //Print("object constructor   count="+objectcount_material);
	}
	~Material(){
		objectcount_material--;
		 //Print("object destructor   count="+objectcount_material);
	}
	
	//methods required for some containers
	//comment out methods to test related exceptions
	bool opEquals(Material &in other){return (name == other.name);}
	int opCmp(Material &in other){
		if(name == other.name){return 0;}
		if(name < other.name){return -1;}else{return 1;}
	}
	aatc_hash_t hash(){return aatc_Hashfunc_djb2(name);}
};