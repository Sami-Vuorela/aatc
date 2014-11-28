void test_auto_iterators(){
	/*
	*/
	{
		Print("auto iterator test for vector<int>");
		vector<int> testvec;
			testvec.push_back(1);
			testvec.push_back(2);
			testvec.push_back(3);
			testvec.push_back(4);
			testvec.push_back(5);
		
		//auto iterator wont crash here
		for(auto it = testvec.begin(); it++;){
			// Print("val = "+it.current());
			// it.current = it.current + 5;
			it.current() += 5;
			Print("val pa = "+it.value);
		}
		Print(" ");
	}
	{
		Print("auto iterator test for vector<int>");
		vector<string> testvec;
			testvec.push_back("1s");
			testvec.push_back("2s");
			testvec.push_back("3s");
			testvec.push_back("4s");
			testvec.push_back("5s");
		
		//auto iterator wont crash here
		for(auto it = testvec.begin(); it++;){
			it.current() += " x";
			it.value = it.value + " xoxo";
			Print("val pa = "+it.value);
		}
		Print(" ");
	}
	{
		Print("auto iterator test for vector<int>");
		set<int> testvec;
			testvec.insert(1);
			testvec.insert(2);
			testvec.insert(3);
			testvec.insert(4);
			testvec.insert(5);
		
		//auto iterator wont crash here
		for(auto it = testvec.begin(); it++;){
			// Print("val = "+it.current());
			// it.testprop = it.testprop + 5;
			Print("val pa = "+it.value);
		}
		Print(" ");
	}
	{
		Print("auto iterator test for set<string>");
		set<string> testvec;
			testvec.insert("1");
			testvec.insert("2");
			testvec.insert("9");
			testvec.insert("3");
			testvec.insert("4");
			
		//auto iterator wont crash here
		for(auto it = testvec.begin(); it++;){
			// Print("val = "+it.current());
			Print("val = "+it.value);
		}
		Print(" ");
	}
	{
		Print("auto iterator test for vector<Material>");
		vector<Material@> testvec;
			testvec.push_back(Material("1s",1));
			testvec.push_back(Material("2s",2));
			testvec.push_back(Material("3s",3));
			testvec.push_back(Material("5s",5));
			testvec.push_back(Material("4s",4));
			
		testvec.sort();
		
		for(auto it = testvec.begin(); it++;){
			// Print("val="+it.current().name);
			it.value = Material(it.value.name + " xoxo",it.value.id);
			// it.value.name += " xoxo";
			Print("val="+it.value.name);
		}
		
		testvec.clear();
		Print(" ");
	}
	
	{
		Print("auto iterator test for map<int,int>");
		map<int,int> testmap;
			testmap.insert(1,15);
			testmap.insert(5,55);
			testmap.insert(2,25);
			
		for(auto it = testmap.begin(); it++;){
		// for(map_iterator<int,int> it(@testmap); it++;){
			// Print("val = "+it.current_key() + " , "+it.current_value());
			Print("val = "+it.key + " , "+it.value);
		}
		Print(" ");
	}
	{
		Print("auto iterator test for map<Material,Material>");
		map<Material,Material> testmap;
			testmap.insert(Material("1s",1),Material("10s",10));
			testmap.insert(Material("2s",2),Material("20s",20));
			testmap.insert(Material("3s",3),Material("30s",30));
			
		for(auto it = testmap.begin(); it++;){
			Print("val = "+it.key.name + " , "+it.value.id);
		}
		Print(" ");
	}
	{
		Print("auto iterator test for unordered_map<Material,Material@>");
		unordered_map<Material,Material@> testmap;
			testmap.insert(Material("1s",1),Material("10s",10));
			testmap.insert(Material("2s",2),Material("20s",20));
			testmap.insert(Material("3s",3),Material("30s",30));
			
		for(auto it = testmap.begin(); it++;){
			it.current_value() = Material(it.value.name,it.key.id + 1000);
			it.value = Material(it.value.name,it.key.id + 100);
			Print("val = "+it.key.name + " , "+it.value.id);
		}
		Print(" ");
	}
	//{
	//	vector<Material> testvec;
	//		testvec.push_back(Material("1",1));
	//		testvec.push_back(Material("2",2));
	//		testvec.push_back(Material("3",3));
	//		testvec.push_back(Material("4",4));
	//		testvec.push_back(Material("5",5));
			
	//	for(auto it = testvec.begin(); it++;){
	//		Print("val="+it.current().id);
	//	}
	//}
}