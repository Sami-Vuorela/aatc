
int sertest_intzorr;
string sertest_stringzorr;

vector<string> sertest_vec_string;
vector<int> sertest_vec_int;
vector<Material> sertest_vec_obj;


void serializer_test_1(){
	sertest_intzorr = 25;
	sertest_stringzorr = "25ss";
	
	sertest_vec_string.push_back("1s");
	sertest_vec_string.push_back("2s");
	sertest_vec_string.push_back("11s");
	sertest_vec_string.push_back("22s");
	
	sertest_vec_int.push_back(1);
	sertest_vec_int.push_back(2);
	sertest_vec_int.push_back(11);
	sertest_vec_int.push_back(22);
	
	sertest_vec_obj.push_back(Material("1s",1));
	sertest_vec_obj.push_back(Material("2s",2));
	sertest_vec_obj.push_back(Material("3s",3));
	sertest_vec_obj.push_back(Material("4s",4));
}
void serializer_test_2(){
	sertest_intzorr = 1125;
	sertest_stringzorr = "1125ss";
	sertest_vec_string.clear();
	sertest_vec_int.clear();
	sertest_vec_obj.clear();
}
void serializer_test_3(){
	Print("sertest int = " + sertest_intzorr);
	Print("sertest string = " + sertest_stringzorr);
	
	Print("------");
	for(auto it = sertest_vec_string.begin(); it++;){
		Print("val = " + it.value);
	}
	Print("------");
	for(auto it = sertest_vec_int.begin(); it++;){
		Print("val = " + it.value);
	}
	Print("------");
	for(auto it = sertest_vec_obj.begin(); it++;){
		Print("val = " + it.value.name);
	}
}