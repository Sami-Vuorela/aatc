
int sertest_intzorr;
string sertest_stringzorr;

vector<string> sertest_vec_string;
vector<int> sertest_vec_int;
vector<Material@> sertest_vec_obj;

set<Material@> sertest_set_obj;

map<int,int> sertest_map_int_int;
map<Material,Material> sertest_map_mat_mat;
map<Material,int> sertest_map_mix;

// vector<vector<Material>> sertest_multimax_vec;
// array<array<Material>> sertest_multimax_array;


//circular reference don't seem to explode the serializer
class Circulator{
	string name;
	vector<Circulator@>@ mylist;
};

vector<Circulator@> circulators;


void serializer_test_1(){
	Print("serializer test 1");
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
	
	sertest_set_obj.insert(Material("1s",1));
	sertest_set_obj.insert(Material("2s",2));
	sertest_set_obj.insert(Material("3s",3));
	sertest_set_obj.insert(Material("4s",4));
	
	circulators.push_back(Circulator());
	circulators.back().name = "name xx";
	@circulators.back().mylist = @circulators;
	
	sertest_map_int_int.insert(2,20);
	sertest_map_int_int.insert(3,30);
	sertest_map_int_int.insert(1,10);
	sertest_map_int_int.insert(4,40);
	
	sertest_map_mat_mat.insert(Material("3s",3),Material("30s",30));
	sertest_map_mat_mat.insert(Material("1s",1),Material("10s",10));
	sertest_map_mat_mat.insert(Material("4s",4),Material("40s",40));
	sertest_map_mat_mat.insert(Material("2s",2),Material("20s",20));
	
	sertest_map_mix.insert(Material("3s",3),20);
	sertest_map_mix.insert(Material("1s",1),40);
	sertest_map_mix.insert(Material("4s",4),10);
	sertest_map_mix.insert(Material("2s",2),30);
	
	// {
		// vector<Material> vex;
		// vex.push_back(Material("1s",1));
		// vex.push_back(Material("2s",2));
		// sertest_multimax_vec.push_back(vex);
	// }
	// {
		// vector<Material> vex;
		// vex.push_back(Material("3s",3));
		// vex.push_back(Material("4s",4));
		// sertest_multimax_vec.push_back(vex);
	// }
	
	// {
		// array<Material> vex;
		// vex.insertLast(Material("1s",1));
		// vex.insertLast(Material("2s",2));
		// sertest_multimax_array.insertLast(vex);
	// }
	// {
		// array<Material> vex;
		// vex.insertLast(Material("3s",3));
		// vex.insertLast(Material("4s",4));
		// sertest_multimax_array.insertLast(vex);
	// }
}
void serializer_test_2(){
	Print("serializer test 2");
	sertest_intzorr = 1125;
	sertest_stringzorr = "1125ss";
	sertest_vec_string.clear();
	sertest_vec_int.clear();
	sertest_vec_obj.clear();
	sertest_set_obj.clear();
	sertest_map_int_int.clear();
	sertest_map_mat_mat.clear();
	sertest_map_mix.clear();
	// sertest_multimax_vec.clear();
	// sertest_multimax_array.resize(0);
	
	circulators.back().name = "empty";
}
void serializer_test_3(){
	Print("serializer test 3");
	
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
	Print("------");
	for(auto it = sertest_set_obj.begin(); it++;){
		Print("val = " + it.value.name);
	}
	Print("------");
	for(auto it = sertest_map_int_int.begin(); it++;){
		Print("val = " + it.key + " , "+ it.value);
	}
	Print("------");
	for(auto it = sertest_map_mat_mat.begin(); it++;){
		Print("val = " + it.key.name + " , "+ it.value.name);
	}
	Print("------");
	for(auto it = sertest_map_mix.begin(); it++;){
		Print("val = " + it.key.name + " , "+ it.value);
	}
	// Print("------");
	// for(auto it = sertest_multimax_vec.begin(); it++;){
		// Print("  ------");
		// for(auto it2 = it.value.begin(); it2++;){
			// Print("    val = " + it2.value.name);
		// }
	// }
	// Print("------");
	// for(int i=0; i<sertest_multimax_array.length; i++ ){
		// Print("  ------");
		// for(int ii=0; ii<sertest_multimax_array[i].length; ii++){
			// Print("    val = " + sertest_multimax_array[i][ii].name);
		// }
	// }
	
	Print("------");
	Print("circulators count = " + circulators.size());
	Print("circulators count 2 = " + circulators.back().mylist.size());
	Print("circulator name = " + circulators.back().mylist[0].name);
}