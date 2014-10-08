void TestContainer_list(){
	{
		test_Startnew("list<int>");
		list<int> cont;
		
		for(int i=0; i<5;i++){
			cont.push_back(i*10);
		}
		cont.remove(30);
		cont.sort(false);
		cont.pop_front();
		cont.pop_back();
		cont.push_back(40);
		cont.push_front(0);
		
		{//test copying the whole container
			list<int> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(list_iterator<int> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current());
		//	i++;
		//}
		if(cont.size() == 4){
			test_success = true;
			list_iterator<int> it(@cont);
			if(it++){if(it.current() != 0){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != 20){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != 10){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != 40){test_success = false;}}else{test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		test_Startnew("list<string>");
		list<string> cont;
		
		for(int i=0; i<5;i++){
			cont.push_back("Xx"+i*10+"xX");
		}
		cont.remove("Xx30xX");
		cont.sort(false);
		cont.pop_front();
		cont.pop_back();
		cont.push_back("Xx40xX");
		cont.push_front("Xx0xX");
		
		{//test copying the whole container
			list<string> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(list_iterator<string> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current());
		//	i++;
		//}
		if(cont.size() == 4){
			test_success = true;
			list_iterator<string> it(@cont);
			if(it++){if(it.current() != "Xx0xX"){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != "Xx20xX"){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != "Xx10xX"){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != "Xx40xX"){test_success = false;}}else{test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		test_Startnew("list<Material>");
		list<Material> cont;
		
		for(int i=0; i<5;i++){
			cont.push_back(Material("Xx"+i*10+"xX",i));
		}
		cont.remove(Material("Xx30xX",0));
		cont.sort(false);
		cont.pop_front();
		cont.pop_back();
		cont.push_back(Material("Xx40xX",0));
		cont.push_front(Material("Xx0xX",0));
		
		{//test copying the whole container
			list<Material> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(list_iterator<Material> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current().name);
		//	i++;
		//}
		if(cont.size() == 4){
			test_success = true;
			list_iterator<Material> it(@cont);
			if(it++){if(it.current() != Material("Xx0xX",0)){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != Material("Xx20xX",0)){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != Material("Xx10xX",0)){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != Material("Xx40xX",0)){test_success = false;}}else{test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		bool first = true;
		
		for(int kk=0; kk<2;kk++){
			if(first){
				test_Startnew("list<Material@>");
			}else{
				test_Startnew("list<Material@> directcomp");
			}
			
			list<Material@> cont;
			
			if(first){
				cont.SetDirectcomp(false);
			}else{
				cont.SetDirectcomp(true);
			}
			
			Material@ m0 = Material("Xx"+0+"xX",0);
			Material@ m1 = Material("Xx"+10+"xX",1);
			Material@ m2 = Material("Xx"+20+"xX",2);
			Material@ m3 = Material("Xx"+30+"xX",3);
			Material@ m4 = Material("Xx"+40+"xX",4);
			
			cont.push_back(m0);
			cont.push_back(m1);
			cont.push_back(m2);
			cont.push_back(m3);
			cont.push_back(m4);
			
			if(first){
				cont.remove(Material("Xx30xX",0));
				cont.sort(false);
			}else{
				cont.remove(m3);
			}
			cont.pop_front();
			cont.pop_back();
			cont.push_back(Material("Xx40xX",0));
			cont.push_front(Material("Xx0xX",0));
			
			{//test copying the whole container
				list<Material@> cont2(cont);
				cont = cont2;
			}
			//int i=0;
			//for(list_iterator<Material@> it(@cont);it++;){
			//	Print("  content["+i+"] = "+it.current().name);
			//	i++;
			//}
			if(cont.size() == 4){
				test_success = true;
				list_iterator<Material@> it(@cont);
				if(first){
					if(it++){if(it.current() != Material("Xx0xX",0)){test_success = false;}}else{test_success = false;}
					if(it++){if(it.current() != Material("Xx20xX",0)){test_success = false;}}else{test_success = false;}
					if(it++){if(it.current() != Material("Xx10xX",0)){test_success = false;}}else{test_success = false;}
					if(it++){if(it.current() != Material("Xx40xX",0)){test_success = false;}}else{test_success = false;}
				}else{
					if(it++){if(it.current() != Material("Xx0xX",0)){test_success = false;}}else{test_success = false;}
					if(it++){if(it.current() != Material("Xx10xX",0)){test_success = false;}}else{test_success = false;}
					if(it++){if(it.current() != Material("Xx20xX",0)){test_success = false;}}else{test_success = false;}
					if(it++){if(it.current() != Material("Xx40xX",0)){test_success = false;}}else{test_success = false;}
				}
			}
			test_PrintSuccess();
			first = false;
		}
	}
}