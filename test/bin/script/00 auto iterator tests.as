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
			testvec.push_back(Material("4s",4));
			testvec.push_back(Material("5s",5));
		
		//error = "Can't implicitly convert from 'vector_iterator<T>' to 'vector_iterator<Material>'."
		//vector_iterator<Material> it = testvec.begin();
		
		auto it = testvec.begin();
		//it = testvec.begin();
		
		//for(auto it = testvec.begin(); it++;){
		for(; it++;){
			Print("val="+it.current().name);
		}
		Print(" ");
	}
	
	//{
	//	map<int,int> testmap;
	//		testmap.insert(1,15);
	//		testmap.insert(2,25);
			
	//	for(auto it = testmap.begin(); it++;){
	//		Print("val = "+it.current_key() + " , "+it.current_value());
	//	}
	//}
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