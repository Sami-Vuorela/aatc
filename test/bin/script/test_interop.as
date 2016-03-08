


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
			Print("test_interop_read_vector_int_handle");
			
			vector<int> cont;
			
			for(int i=0; i<10; i++){
				cont.push_back(i);
			}

			test_interop_read_vector_int_handle(cont);
			
			PrintTabDecrement();
		PrintTabDecrement();
		Print("");
	}
	{
		PrintTabIncrement();
			Print("test_interop_read_vector_int_ref");
			
			vector<int> cont;
			
			for(int i=0; i<10; i++){
				cont.push_back(i);
			}

			test_interop_read_vector_int_ref(cont);
			
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