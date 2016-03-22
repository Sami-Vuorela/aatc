//delegate
class lambdatest_Comparizor{
	int counter = 0;
	
	int comp(int l,int r){
		counter++;
		
		if(l<r){
			return -1;
		}else if(l>r){
			return 1;
		}
		return 0;
	}
}



void Test_lambda(){
	{//sort with lambda
		vector<int> cont;
		
		for(int i=10; i>0; i--){
			cont.push_back(i);
		}
		for(int i=10; i>0; i--){
			cont.push_back(i);
		}
		
		cont.sort(function(l,r){
			if(l < r){
				return -1;
			}
			if(l > r){
				return 1;
			}
			return 0;
		},true);
		
		// Print(""); Print("------------");
		// for(auto it = cont.begin();it++;){
			// Print("it.value = " + it.value);
		// }
	}
	{//sort with delegate
		vector<int> cont;
		
		for(int i=10; i>0; i--){
			cont.push_back(i);
		}
		for(int i=10; i>0; i--){
			cont.push_back(i);
		}
		
		lambdatest_Comparizor compa;
		cont.sort(aatc_funcdef_cmp_int(compa.comp),true);
		
		// Print(""); Print("------------");
		// for(auto it = cont.begin();it++;){
			// Print("it.value = " + it.value);
		// }
		// Print("Delegate counter = " + compa.counter);
	}
	{//sort with aatcfuncptr delegate
		vector<int> cont;
		
		for(int i=10; i>0; i--){
			cont.push_back(i);
		}
		for(int i=10; i>0; i--){
			cont.push_back(i);
		}
		
		
		lambdatest_Comparizor compa;
		
		aatc_funcpointer fp;
		fp.Set("comp",@compa);
		
		cont.sort(fp,true);
		
		// Print(""); Print("------------");
		// for(auto it = cont.begin();it++;){
			// Print("it.value = " + it.value);
		// }
		// Print("Delegate counter = " + compa.counter);
	}
	
	{//sort with lambda
		vector<string> cont;
		
		for(int i=10; i>0; i--){
			cont.push_back("" +i + "s");
		}
		
		cont.sort(function(l,r){
			if(l < r){
				return -1;
			}
			if(l > r){
				return 1;
			}
			return 0;
		},true);
		
		// Print(""); Print("------------");
		// for(auto it = cont.begin();it++;){
			// Print("it.value = " + it.value);
		// }
	}
	{
		vector<Material> cont;
		
		for(int i=10; i>0; i--){
			cont.push_back(Material("" + i + "s",i));
		}
		
		//not yet available for templated containers
		// cont.sort(function(l,r){
			// if(l.id < r.id){
				// return -1;
			// }
			// if(l.id > r.id){
				// return 1;
			// }
			// return 0;
		// },true);
		
		// Print(""); Print("------------");
		// for(auto it = cont.begin();it++;){
			// Print("it.value.id = " + it.value.id);
		// }
	}
}