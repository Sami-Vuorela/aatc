void randomtest_iterators(){
	{//test container::end() and other iterator things
		Print("");
		Print("test container::end()");
		
		map<int,int> cont;
		
		cont[1] = 11;
		cont[3] = 33;
		cont[2] = 22;
		cont[4] = 44;
		cont[5] = 55;
		
		{
			auto it = cont.begin();
			auto it_end = cont.end();
			
			Print("it IsEnd() = "+it.IsEnd());
			Print("it equals end = "+(it == it_end));
			Print("it equals end = "+(it == cont.end()));
			
			for(;it++;){
				Print("value = "+it.value);
			}
			
			Print("it IsEnd() = "+it.IsEnd());
			Print("it equals end = "+(it == it_end));
			Print("it equals end = "+(it == cont.end()));
		}
		
		{
			auto it_find = cont.find_iterator(3);
			
			if(it_find.IsEnd()){
				Print("it find not found");
			}else{
				Print("it find value = "+it_find.value);
			}
			
		}
		
		{
			auto it_range_begin = cont.find_iterator(2);
			auto it_range_end = cont.find_iterator(4);
			//cont.erase(cont.find(2),cont.find(4));
			cont.erase(it_range_begin,it_range_end);
			//cont.erase(cont.find_iterator(2));
			//cont.erase(cont.find_iterator(4));
			
			for(auto it = cont.begin();it++;){
				Print("value = "+it.value);
			}
		}
		
	}
	
	
	{//test container::end() and other iterator things
		Print("");
		Print("test container::end()");
		
		unordered_map<string,string> cont;
		
		cont["1s"] = "11x";
		cont["3sss"] = "33x";
		cont["2ss"] = "22x";
		cont["4ssss"] = "44x";
		
		auto it = cont.begin();
		auto it_end = cont.end();
		
		Print("it IsEnd() = "+it.IsEnd());
		Print("it equals end = "+(it == it_end));
		Print("it equals end = "+(it == cont.end()));
		
		for(;it++;){
			Print("value = "+it.value);
		}
		
		Print("it IsEnd() = "+it.IsEnd());
		Print("it equals end = "+(it == it_end));
		Print("it equals end = "+(it == cont.end()));
		
		
		{
			auto it_find = cont.find_iterator("3sss");
			
			if(it_find.IsEnd()){
				Print("it find not found");
			}else{
				Print("it find value = "+it_find.value);
			}
			
			cont.erase(it_find);
			
			it_find = cont.find_iterator("3sss");
			
			if(it_find.IsEnd()){
				Print("it find not found");
			}else{
				Print("it find value = "+it_find.value);
			}
		}
		
		{//find and iterate from there
			Print("");
			auto it_find = cont.find_iterator("3sss");
			
			for(;it_find++;){
				Print("it find value = "+it_find.value);
			}
		}
		
		
	}
	
	{
		Print("test erase range");
		vector<string> cont;
		
		cont.push_back("1s");
		cont.push_back("2s");
		cont.push_back("3s");
		cont.push_back("4s");
		cont.push_back("5s");
		cont.push_back("6s");
		cont.push_back("7s");
		cont.push_back("8s");
		cont.push_back("9s");
		
		{
		vector_iterator<string> it_range_begin;
		vector_iterator<string> it_range_end;
		
		for(auto it = cont.begin(); it++;){
			if(it.value == "3s"){
				it_range_begin = it;
			}
			if(it.value == "7s"){
				it_range_end = it;
			}
		}
		
		cont.erase(it_range_begin,it_range_end);
		}
		
		for(auto it = cont.begin(); it++;){
			Print("value = " + it.value);
		}
		
		
	}
	
	{
		Print("test erase range");
		vector<Material> cont;
		
		//Material@ mat1 = Material("2oo",0);
		//Material@ mat2 = Material("6oo",0);
		
		cont.push_back(Material("1o",0));
		cont.push_back(Material("2o",0));
		//cont.push_back(mat1);
		cont.push_back(Material("3o",0));
		cont.push_back(Material("4o",0));
		cont.push_back(Material("5o",0));
		cont.push_back(Material("6o",0));
		//cont.push_back(mat2);
		cont.push_back(Material("7o",0));
		cont.push_back(Material("8o",0));
		cont.push_back(Material("9o",0));
		
		//vector_iterator<Material@> it_range_begin;
		//vector_iterator<Material@> it_range_end;
		
		//for(auto it = cont.begin(); it++;){
		//	if(it.value.name == "3o"){
		//		it_range_begin = it;
		//	}
		//	if(it.value.name == "7o"){
		//		it_range_end = it;
		//	}
		//}
		
		//cont.erase_value(Material("3o",0));
		
		auto it_range_begin = cont.find_iterator(Material("3o",0));
		auto it_range_end = cont.find_iterator(Material("7o",0));
		//auto it_range_begin = cont.find_iterator(@mat1);
		//auto it_range_end = cont.find_iterator(@mat2);
		
		cont.erase(it_range_begin,it_range_end);
		
		cont.insert(cont.find_iterator(Material("9o",0)),Material("insert iterator woo",0));
			
		for(auto it = cont.begin(); it++;){
		
		
			Print("value = " + it.value.name);
		}
		
		
	}
	
	{
		Print("test erase range");
		set<int> cont;
		
		cont.insert(1);
		cont.insert(2);
		cont.insert(3);
		cont.insert(4);
		cont.insert(5);
		cont.insert(6);
		cont.insert(7);
		cont.insert(8);
		cont.insert(9);
		
			auto it_range_begin = cont.find_iterator(3);
			auto it_range_end = cont.find_iterator(7);
			
			//auto it_range_begin = cont.begin();
			//auto it_range_end = cont.begin();
			
			//for(auto it = cont.begin(); it++;){
			//	if(it.value == 3){
			//		it_range_begin = it;
			//	}
			//	if(it.value == 7){
			//		it_range_end = it;
			//	}
			//}
			
			//cont.erase(it_range_begin);
			//cont.erase(it_range_begin,it_range_end);
			Print("erase range count = " + cont.erase(it_range_begin,it_range_end));
			//Print("erase range count = " + cont.erase(it_range_begin,it_range_begin));
			//Print("erase range count = " + cont.erase(cont.begin(),cont.begin()));
		
		{
		for(auto it = cont.begin(); it++;){
			Print("value = " + it.value);
		}
		}
		
		
	}
	
	{
		Print("test erase range");
		vector<int> cont;
		
		cont.push_back(1);
		cont.push_back(2);
		cont.push_back(3);
		cont.push_back(4);
		cont.push_back(5);
		cont.push_back(6);
		cont.push_back(7);
		cont.push_back(8);
		cont.push_back(9);
		
			vector_iterator<int> it_range_begin;
			vector_iterator<int> it_range_end;
			
			for(auto it = cont.begin(); it++;){
				if(it.value == 3){
					it_range_begin = it;
				}
				if(it.value == 7){
					it_range_end = it;
				}
			}
			
			cont.erase(it_range_begin,it_range_end);
		
		{
			for(auto it = cont.begin(); it++;){
				Print("value = " + it.value);
			}
		}
		
		
	}
}