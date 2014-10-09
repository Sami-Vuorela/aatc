void TestContainer_vector(){
	{
		test_Startnew("vector<int>");
		
		vector<int> cont;
		
		for(int i=0; i<5;i++){
			cont.push_back(i*10);
		}
		cont.erase_index(3);
		
		cont.sort(false);
		
		{//test copying the whole container
			vector<int> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(vector_iterator<int> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current());
		//	i++;
		//}
		//{
		//	Print("  test primitive vector copy");
		//	vector<int> cont2 = cont;
			
		//	int i=0;
		//	for(vector_iterator<int> it(@cont2);it++;){
		//		Print("    copy content["+i+"] = "+it.current());
		//		i++;
		//	}
		//}
		if(cont.size() == 4){
			if((cont[0] == 40) && (cont[1] == 20) && (cont[2] == 10) && (cont[3] == 0)){
				test_success = true;
			}
		}
		test_PrintSuccess();
		//cont.clear();
	}
	{
		test_Startnew("vector<string>");
		
		vector<string> cont;
		
		for(int i=0; i<5;i++){
			cont.push_back(""+(i*10));
		}
		cont.erase_index(3);
		
		for(vector_iterator<string> it(@cont);it++;){
			it.current() = "Xx"+it.current()+"xX";
		}
		
		cont.sort(false);
		
		{//test copying the whole container
			vector<string> cont2(cont);
			cont = cont2;
		}
		//{
		//	Print("  test primitive string vector copy");
		//	vector<string> cont2(cont);
			
		//	cont2.sort(true);
			
		//	int i=0;
		//	for(vector_iterator<string> it(@cont2);it++;){
		//		Print("    copy content["+i+"] = "+it.current());
		//		i++;
		//	}
		//}
		
		//int i=0;
		//for(vector_iterator<string> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current());
		//	i++;
		//}
		if(cont.size() == 4){
			if((cont[0] == "Xx40xX") && (cont[1] == "Xx20xX") && (cont[2] == "Xx10xX") && (cont[3] == "Xx0xX")){
				test_success = true;
			}
		}
		test_PrintSuccess();
		//cont.clear();
	}
	{
		test_Startnew("vector<Material>");
		
		vector<Material> cont;
		
		for(int i=0; i<5;i++){
			Material a("Xx"+(i*10)+"xX",i);
			cont.push_back(a);
		}
		cont.erase_index(3);
		
		cont.sort(false);
		
		{//test copying the whole container
			vector<Material> cont2(cont);
			cont = cont2;
		}
		//{
		//	vector<Material> cont2 = cont;
			
		//	int i=0;
		//	for(vector_iterator<Material> it(@cont2);it++;){
		//		Print("    copy content["+i+"] = "+it.current().name + " , "+it.current().id);
		//		i++;
		//	}
		//}
		//int i=0;
		//for(vector_iterator<Material> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current().name + " , "+it.current().id);
		//	i++;
		//}
		if(cont.size() == 4){
			if((cont[0].name == "Xx40xX") && (cont[1].name == "Xx20xX") && (cont[2].name == "Xx10xX") && (cont[3].name == "Xx0xX")){
				test_success = true;
			}
		}
		test_PrintSuccess();
		//cont.clear();
	}
	bool first = true;
	for(int kk=0; kk<2;kk++){
		if(first){
			test_Startnew("vector<Material@>");
		}else{
			test_Startnew("vector<Material@> directcomp");
		}
		
		vector<Material@> cont;
		
		if(!first){
			cont.SetDirectcomp(true);
		}
		first = false;
		
		for(int i=0; i<5;i++){
			Material@ a = Material("Xx"+(i*10)+"xX",i);
			cont.push_back(a);
		}
		cont.erase_index(3);
		
		cont.sort(false);
		
		{//test copying the whole container
			vector<Material@> cont2(cont);
			cont = cont2;
		}
		//int i=0;
		//for(vector_iterator<Material@> it(@cont);it++;){
		//	Print("  content["+i+"] = "+it.current().name + " , "+it.current().id);
		//	i++;
		//}
		if(cont.size() == 4){
			//if((cont[0].name == "Xx40xX") && (cont[1].name == "Xx20xX") && (cont[2].name == "Xx10xX") && (cont[3].name == "Xx0xX")){
				test_success = true;
			//}
		}
		test_PrintSuccess();
		//cont.clear();
	}
}