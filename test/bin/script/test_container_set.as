void TestContainer_set(){
	{
		test_Startnew("set<int>");
		set<int> cont;
		
		for(int i=0; i<5;i++){
			cont.insert(i*10);
		}
		cont.erase(30);
		
		{//test copying the whole container
			set<int> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(set_iterator<int> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current());
		//	i++;
		//}
		if(cont.size() == 4){
			test_success = true;
			set_iterator<int> it(@cont);
			if(it++){if(it.current() != 0){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != 10){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != 20){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != 40){test_success = false;}}else{test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		test_Startnew("set<string>");
		set<string> cont;
		
		for(int i=0; i<5;i++){
			cont.insert("Xx"+i*10+"xX");
		}
		cont.erase("Xx30xX");
		
		{//test copying the whole container
			set<string> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(set_iterator<string> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current());
		//	i++;
		//}
		if(cont.size() == 4){
			test_success = true;
			set_iterator<string> it(@cont);
			if(it++){if(it.current() != "Xx0xX"){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != "Xx10xX"){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != "Xx20xX"){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != "Xx40xX"){test_success = false;}}else{test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		test_Startnew("set<Material>");
		set<Material> cont;
		
		for(int i=0; i<5;i++){
			cont.insert(Material("Xx"+i*10+"xX",i));
		}
		cont.erase(Material("Xx30xX",0));
		
		{//test copying the whole container
			set<Material> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(set_iterator<Material> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current().name);
		//	i++;
		//}
		if(cont.size() == 4){
			test_success = true;
			set_iterator<Material> it(@cont);
			if(it++){if(it.current() != Material("Xx0xX",0)){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != Material("Xx10xX",0)){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != Material("Xx20xX",0)){test_success = false;}}else{test_success = false;}
			if(it++){if(it.current() != Material("Xx40xX",0)){test_success = false;}}else{test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		bool first = true;
		
		for(int kk=0; kk<2;kk++){
			if(first){
				test_Startnew("set<Material@>");
			}else{
				test_Startnew("set<Material@> directcomp");
			}
			
			set<Material@> cont;
			
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
			
			cont.insert(m0);
			cont.insert(m1);
			cont.insert(m2);
			cont.insert(m3);
			cont.insert(m4);
			
			if(first){
				cont.erase(Material("Xx30xX",0));
			}else{
				cont.erase(m3);
			}
			
			{//test copying the whole container
				set<Material@> cont2(cont);
				cont = cont2;
			}
			//int i=0;
			//for(set_iterator<Material@> it(@cont);it++;){
			//	Print("  content["+i+"] = "+it.current().name);
			//	i++;
			//}
			if(cont.size() == 4){
				test_success = true;
				if(first){
					set_iterator<Material@> it(@cont);
					if(it++){if(it.current() != Material("Xx0xX",0)){test_success = false;}}else{test_success = false;}
					if(it++){if(it.current() != Material("Xx10xX",0)){test_success = false;}}else{test_success = false;}
					if(it++){if(it.current() != Material("Xx20xX",0)){test_success = false;}}else{test_success = false;}
					if(it++){if(it.current() != Material("Xx40xX",0)){test_success = false;}}else{test_success = false;}
				}else{
					if(!cont.contains(m0)){test_success = false;}
					if(!cont.contains(m1)){test_success = false;}
					if(!cont.contains(m2)){test_success = false;}
					if(!cont.contains(m4)){test_success = false;}
				}
			}
			test_PrintSuccess();
			first = false;
		}
	}
}