void TestContainer_unordered_set(){
	{
		test_Startnew("unordered_set<int>");
		unordered_set<int> cont;
		
		for(int i=0; i<5;i++){
			cont.insert(i*10);
		}
		cont.erase(30);
		
		{//test copying the whole container
			unordered_set<int> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(unordered_set_iterator<int> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current());
		//	i++;
		//}
		if(cont.size() == 4){
			test_success = true;
			if(!cont.contains(0)){test_success = false;}
			if(!cont.contains(10)){test_success = false;}
			if(!cont.contains(20)){test_success = false;}
			if(!cont.contains(40)){test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		test_Startnew("unordered_set<string>");
		unordered_set<string> cont;
		
		for(int i=0; i<5;i++){
			cont.insert("Xx"+i*10+"xX");
		}
		cont.erase("Xx30xX");
		
		{//test copying the whole container
			unordered_set<string> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(unordered_set_iterator<string> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current());
		//	i++;
		//}
		if(cont.size() == 4){
			test_success = true;
			if(!cont.contains("Xx0xX")){test_success = false;}
			if(!cont.contains("Xx10xX")){test_success = false;}
			if(!cont.contains("Xx20xX")){test_success = false;}
			if(!cont.contains("Xx40xX")){test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		test_Startnew("unordered_set<Material>");
		unordered_set<Material> cont;
		
		for(int i=0; i<5;i++){
			cont.insert(Material("Xx"+i*10+"xX",i));
		}
		cont.erase(Material("Xx30xX",0));
		
		{//test copying the whole container
			unordered_set<Material> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(unordered_set_iterator<Material> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current().name);
		//	i++;
		//}
		if(cont.size() == 4){
			test_success = true;
			if(!cont.contains(Material("Xx0xX",0))){test_success = false;}
			if(!cont.contains(Material("Xx10xX",0))){test_success = false;}
			if(!cont.contains(Material("Xx20xX",0))){test_success = false;}
			if(!cont.contains(Material("Xx40xX",0))){test_success = false;}
		}
		test_PrintSuccess();
	}
	{
		bool first = true;
		
		for(int kk=0; kk<2;kk++){
			if(first){
				test_Startnew("unordered_set<Material@>");
			}else{
				test_Startnew("unordered_set<Material@> directcomp");
			}
			
			unordered_set<Material@> cont;
			
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
				unordered_set<Material@> cont2(cont);
				cont = cont2;
			}
			
			//int i=0;
			//for(unordered_set_iterator<Material@> it(@cont);it++;){
			//	Print("  content["+i+"] = "+it.current().name);
			//	i++;
			//}
			if(cont.size() == 4){
				test_success = true;
				unordered_set_iterator<Material@> it(@cont);
				if(first){
					if(!cont.contains(Material("Xx0xX",0))){test_success = false;}
					if(!cont.contains(Material("Xx10xX",0))){test_success = false;}
					if(!cont.contains(Material("Xx20xX",0))){test_success = false;}
					if(!cont.contains(Material("Xx40xX",0))){test_success = false;}
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