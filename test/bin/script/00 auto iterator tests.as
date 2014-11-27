void test_auto_iterators(){
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
			Print("val = "+it.current());
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
			Print("val = "+it.current());
		}
		Print(" ");
	}
	{
		Print("auto iterator test for vector<Material>");
		vector<Material> testvec;
			testvec.push_back(Material("1s",1));
			testvec.push_back(Material("2s",2));
			testvec.push_back(Material("3s",3));
			testvec.push_back(Material("5s",5));
			testvec.push_back(Material("4s",4));
			
		testvec.sort();
		
		for(auto it = testvec.begin(); it++;){
			Print("val="+it.current().name);
		}
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
			Print("val = "+it.current_key() + " , "+it.current_value());
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
			Print("val = "+it.current_key().name + " , "+it.current_value().id);
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
			Print("val = "+it.current_key().name + " , "+it.current_value().id);
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