void TestContainer_unordered_map(){
	{
		test_Startnew("unordered_map<int,int>");
		test_success = true;
		
		unordered_map<int,int> cont;
		
		cont.insert(1,10);
		cont.insert(2,20);
		cont.insert(5,50);
		
		cont.erase(3);
		cont.erase(5);
		
		// cont.insert(4,40);
		// cont.insert(5,50);
		
		//test inserting new things with opIndex
		cont[4] = 40;
		cont[5] = 50;
		
		{//test copying the whole container
			unordered_map<int,int> cont2(cont);
			cont = cont2;
		}
		{
			bool success = false;
			int findresult = cont.find(2,success);
			if(success){
				if(findresult != 20){
					test_success = false;
					test_fail_reason = "find returned wrong answer";
				}
			}else{
				test_success = false;
				test_fail_reason = "find found nothing";
			}
		}
		
		//for(unordered_map_iterator<int,int> it(@cont); it++;){
		//	Print("  content = "+it.current_key()+" , "+it.current_value());
		//}
		
		if(cont.size() == 4){
			if(!cont.contains(1)){test_success = false;}
			if(!cont.contains(2)){test_success = false;}
			if(!cont.contains(4)){test_success = false;}
			if(!cont.contains(5)){test_success = false;}
		}
		test_PrintSuccess();
	}
	//if(false){
	{
		test_Startnew("unordered_map<string,float>");
		test_success = true;
		
		unordered_map<string,float> cont;
		
		cont.insert("1s",1.234);
		cont.insert("2s",2.345);
		cont.insert("5s",5.678);
		
		cont.erase("3s");
		cont.erase("5s");
		
		cont.insert("4s",4.567);
		cont.insert("5s",5.678);
		
		{//test copying the whole container
			unordered_map<string,float> cont2(cont);
			cont = cont2;
		}
		{
			bool success = false;
			float findresult = cont.find("2s",success);
			if(success){
				//test opIndex
				findresult = cont["2s"];
				if(findresult != 2.345){
					test_success = false;
					test_fail_reason = "find returned wrong answer";
				}
			}else{
				test_success = false;
				test_fail_reason = "find found nothing";
			}
		}
		
		//for(unordered_map_iterator<string,float> it(@cont); it++;){
		//	Print("  content = "+it.current_key()+" , "+it.current_value());
		//}
		
		if(cont.size() == 4){
			if(!cont.contains("1s")){test_success = false;}
			if(!cont.contains("2s")){test_success = false;}
			if(!cont.contains("4s")){test_success = false;}
			if(!cont.contains("5s")){test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		test_Startnew("unordered_map<Material,string>");
		test_success = true;
		
		unordered_map<Material,string> cont;
		
		cont.insert(Material("1s",11),"1.234s");
		cont.insert(Material("2s",22),"2.345s");
		cont.insert(Material("5s",55),"5.678s");
		
		cont.erase(Material("3s",9999));
		cont.erase(Material("5s",55));
		
		// cont.insert(Material("4s",44),"4.567s");
		// cont.insert(Material("5s",55),"5.678s");
		
		//test inserting new things with opIndex
		cont[Material("4s",44)] = "4.567s";
		cont[Material("5s",55)] = "5.678s";
		
		{//test copying the whole container
			unordered_map<Material,string> cont2(cont);
			cont = cont2;
		}
		{
			bool success = false;
			string findresult = cont.find(Material("2s",22),success);
			if(success){
				//test opIndex
				findresult = cont[Material("2s",22)];
				if(findresult != "2.345s"){
					test_success = false;
					test_fail_reason = "find returned wrong answer";
				}
			}else{
				test_success = false;
				test_fail_reason = "find found nothing";
			}
		}
		
		//for(unordered_map_iterator<Material,string> it(@cont); it++;){
		//	Print("  content = "+it.current_key().name+" , "+it.current_value());
		//}
		
		if(cont.size() == 4){
			if(!cont.contains(Material("1s",11))){test_success = false;}
			if(!cont.contains(Material("2s",22))){test_success = false;}
			if(!cont.contains(Material("4s",44))){test_success = false;}
			if(!cont.contains(Material("5s",55))){test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		bool first = true;
		
		for(int kk=0; kk<2;kk++){
			if(first){
				test_Startnew("unordered_map<Material@,bool>");
			}else{
				test_Startnew("unordered_map<Material@,bool> directcomp");
			}
			test_success = true;
			
			unordered_map<Material@,bool> cont;
			if(first){
				cont.SetDirectcomp(false);
			}else{
				cont.SetDirectcomp(true);
			}
			
			Material@ m1 = Material("1s",11);
			Material@ m2 = Material("2s",22);
			Material@ m4 = Material("4s",44);
			Material@ m5 = Material("5s",55);
			
			cont.insert(m1,true);
			cont.insert(m2,false);
			cont.insert(m5,true);
			
			cont.erase(Material("3s",9999));
			//cont.erase(Material("5s",55));
			
			if(first){
				cont.erase(Material("5s",55));
			}else{
				cont.erase(m5);
			}
			
			cont.insert(m4,false);
			cont.insert(m5,true);
			
			{//test copying the whole container
				unordered_map<Material@,bool> cont2(cont);
				cont = cont2;
			}
			{
				bool success = false;
				bool findresult = cont.find(m2,success);
				if(success){
					if(findresult != false){
						test_success = false;
						test_fail_reason = "find returned wrong answer";
					}
				}else{
					test_success = false;
					test_fail_reason = "find found nothing";
				}
			}
			
			//for(unordered_map_iterator<Material@,bool> it(@cont); it++;){
			//	Print("  content = "+it.current_key().name+" , "+it.current_value());
			//}
			
			if(cont.size() == 4){
				if(first){
					if(!cont.contains(Material("1s",11))){test_success = false;}
					if(!cont.contains(Material("2s",22))){test_success = false;}
					if(!cont.contains(Material("4s",44))){test_success = false;}
					if(!cont.contains(Material("5s",55))){test_success = false;}
				}else{
					if(!cont.contains(m1)){test_success = false;}
					if(!cont.contains(m2)){test_success = false;}
					if(!cont.contains(m4)){test_success = false;}
					if(!cont.contains(m5)){test_success = false;}
				}
			}
			test_PrintSuccess();
			first = false;
		}
	}
}