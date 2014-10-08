void TestContainer_map(){
	{
		test_Startnew("map<int,int>");
		test_success = true;
		
		map<int,int> cont;
		
		cont.insert(1,10);
		cont.insert(2,20);
		cont.insert(5,50);
		
		cont.erase(3);
		cont.erase(5);
		
		cont.insert(4,40);
		cont.insert(5,50);
		
		{//test copying the whole container
			map<int,int> cont2(cont);
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
		
		//for(map_iterator<int,int> it(@cont); it++;){
		//	Print("  content = "+it.current_key()+" , "+it.current_value());
		//}
		
		if(cont.size() == 4){
			map_iterator<int,int> it(@cont);
			if(it++){if((it.current_key() != 1) || (it.current_value() != 10)){test_success = false;}}else{test_success = false;}
			if(it++){if((it.current_key() != 2) || (it.current_value() != 20)){test_success = false;}}else{test_success = false;}
			if(it++){if((it.current_key() != 4) || (it.current_value() != 40)){test_success = false;}}else{test_success = false;}
			if(it++){if((it.current_key() != 5) || (it.current_value() != 50)){test_success = false;}}else{test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		test_Startnew("map<string,float>");
		test_success = true;
		
		map<string,float> cont;
		
		cont.insert("1s",1.234);
		cont.insert("2s",2.345);
		cont.insert("5s",5.678);
		
		cont.erase("3s");
		cont.erase("5s");
		
		cont.insert("4s",4.567);
		cont.insert("5s",5.678);
		
		{//test copying the whole container
			map<string,float> cont2(cont);
			cont = cont2;
		}
		{
			bool success = false;
			float findresult = cont.find("2s",success);
			if(success){
				if(findresult != 2.345){
					test_success = false;
					test_fail_reason = "find returned wrong answer";
				}
			}else{
				test_success = false;
				test_fail_reason = "find found nothing";
			}
		}
		
		//for(map_iterator<string,float> it(@cont); it++;){
		//	Print("  content = "+it.current_key()+" , "+it.current_value());
		//}
		
		if(cont.size() == 4){
			map_iterator<string,float> it(@cont);
			if(it++){if((it.current_key() != "1s") || (it.current_value() != 1.234)){test_success = false;}}else{test_success = false;}
			if(it++){if((it.current_key() != "2s") || (it.current_value() != 2.345)){test_success = false;}}else{test_success = false;}
			if(it++){if((it.current_key() != "4s") || (it.current_value() != 4.567)){test_success = false;}}else{test_success = false;}
			if(it++){if((it.current_key() != "5s") || (it.current_value() != 5.678)){test_success = false;}}else{test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		test_Startnew("map<Material,string>");
		test_success = true;
		
		map<Material,string> cont;
		
		cont.insert(Material("1s",11),"1.234s");
		cont.insert(Material("2s",22),"2.345s");
		cont.insert(Material("5s",55),"5.678s");
		
		cont.erase(Material("3s",9999));
		cont.erase(Material("5s",55));
		
		cont.insert(Material("4s",44),"4.567s");
		cont.insert(Material("5s",55),"5.678s");
		
		{//test copying the whole container
			map<Material,string> cont2(cont);
			cont = cont2;
		}
		{
			bool success = false;
			string findresult = cont.find(Material("2s",22),success);
			if(success){
				if(findresult != "2.345s"){
					test_success = false;
					test_fail_reason = "find returned wrong answer";
				}
			}else{
				test_success = false;
				test_fail_reason = "find found nothing";
			}
		}
		
		//for(map_iterator<Material,string> it(@cont); it++;){
		//	Print("  content = "+it.current_key().name+" , "+it.current_value());
		//}
		
		if(cont.size() == 4){
			map_iterator<Material,string> it(@cont);
			if(it++){if((it.current_key().name != "1s") || (it.current_value() != "1.234s")){test_success = false;}}else{test_success = false;}
			if(it++){if((it.current_key().name != "2s") || (it.current_value() != "2.345s")){test_success = false;}}else{test_success = false;}
			if(it++){if((it.current_key().name != "4s") || (it.current_value() != "4.567s")){test_success = false;}}else{test_success = false;}
			if(it++){if((it.current_key().name != "5s") || (it.current_value() != "5.678s")){test_success = false;}}else{test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		bool first = true;
		
		for(int kk=0; kk<2;kk++){
			if(first){
				test_Startnew("map<Material@,bool>");
			}else{
				test_Startnew("map<Material@,bool> directcomp");
			}
			test_success = true;
			
			map<Material@,bool> cont;
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
				map<Material@,bool> cont2(cont);
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
			
			//for(map_iterator<Material@,bool> it(@cont); it++;){
			//	Print("  content = "+it.current_key().name+" , "+it.current_value());
			//}
			
			if(cont.size() == 4){
				if(first){
					map_iterator<Material@,bool> it(@cont);
					if(it++){if((it.current_key().name != "1s") || (it.current_value() != true)){test_success = false;}}else{test_success = false;}
					if(it++){if((it.current_key().name != "2s") || (it.current_value() != false)){test_success = false;}}else{test_success = false;}
					if(it++){if((it.current_key().name != "4s") || (it.current_value() != false)){test_success = false;}}else{test_success = false;}
					if(it++){if((it.current_key().name != "5s") || (it.current_value() != true)){test_success = false;}}else{test_success = false;}
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