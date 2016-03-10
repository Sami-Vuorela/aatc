


void test_interop_main(){
	Print("");
	Print("");
	Print("");
	Print("test_interop_main");
	PrintTabIncrement();
	
	Print("test_interop_main :: write");
	{
		PrintTabIncrement();
			Print("test_interop_write_vector_int_handle");
			
			vector<int>@ cont = test_interop_write_vector_int_handle();
			
			PrintTabIncrement();
			for(auto it = cont.begin();it++;){
				Print("it.value = " + it.value);
			}
			
			PrintTabDecrement();
		PrintTabDecrement();
		Print("");
	}
	{
		PrintTabIncrement();
			Print("test_interop_write_map_int_int_handle");
			
			map<int,int>@ cont = test_interop_write_map_int_int_handle();
			
			PrintTabIncrement();
			for(auto it = cont.begin();it++;){
				Print("it.key = " + it.key + "  it.value = " + it.value);
			}
			PrintTabDecrement();
		
		PrintTabDecrement();
		Print("");
	}
	
	Print("");
	Print("");
	Print("");
	Print("test_interop_main :: read");
	{
		PrintTabIncrement();
			Print("test_interop_read_vector_int");
			
			vector<int> cont;
			
			for(int i=0; i<10; i++){
				cont.push_back(i);
			}

			test_interop_read_vector_int(cont);
			
			PrintTabDecrement();
		PrintTabDecrement();
		Print("");
	}
	{
		PrintTabIncrement();
			Print("test_interop_read_map_int_int");
			
			map<int,int> cont;
			
			for(int i=0; i<5;i++){
				cont.insert(i,100 + i);
			}

			test_interop_read_map_int_int(cont);
			
			PrintTabDecrement();
		PrintTabDecrement();
		Print("");
	}
	{
		PrintTabIncrement();
			Print("test_interop_read_map_string_string");
			
			map<string,string> cont;
			
			for(int i=0; i<5;i++){
				cont.insert("s" + i,"ss" + 100 + i);
			}

			test_interop_read_map_string_string(cont);
			
			PrintTabDecrement();
		PrintTabDecrement();
		Print("");
	}
	
	PrintTabDecrement();
}

vector<int>@ test_interop_script_creates_vector_int(){
	vector<int> cont;
	
	for(int i=0; i<10;i++){
		cont.push_back(i);
	}
	
	return cont;
}
vector<int>@ test_interop_script_creates_vector_int_handle(){
	vector<int>@ cont = vector<int>();
	
	for(int i=0; i<10;i++){
		cont.push_back(i);
	}
	
	return @cont;
}

void test_interop_script_reads_vector_int_handle(vector<int>@ cont){
	Print("test_interop_script_reads_vector_int_handle");
	
	PrintTabIncrement();
	for(auto it = cont.begin();it++;){
		Print("it.value = " + it.value);
	}
	PrintTabDecrement();
	
	Print("");
}
void test_interop_script_reads_map_int_int_handle(map<int,int>@ cont){
	Print("test_interop_script_reads_map_int_int_handle");
	
	PrintTabIncrement();
	for(auto it = cont.begin();it++;){
		Print("it.key = " + it.key + "  it.value = " + it.value);
	}
	PrintTabDecrement();
	
	Print("");
}
void test_interop_script_reads_map_string_string_handle(map<string,string>@ cont){
	Print("test_interop_script_reads_map_string_string_handle");
	
	PrintTabIncrement();
	for(auto it = cont.begin();it++;){
		Print("it.key = " + it.key + "  it.value = " + it.value);
	}
	PrintTabDecrement();
	
	Print("");
}