
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

class Container_of_containers{
	vector<int> my_vec_int;
	map<int,int> my_map_int_int;
	unordered_set<int> my_uoset_int;
	Container_of_containers@ coc_friend;
	
	void clear(){
		my_vec_int.clear();
		my_map_int_int.clear();
		my_uoset_int.clear();
		@coc_friend = null;
	}
};
Container_of_containers coc;
vector<Container_of_containers@> cocs;

class sertest_Host{
	vector<sertest_World@> worlds;
};
class sertest_World{
	sertest_Host@ host;//test circular reference
	vector<sertest_WorldObject@> objects;
	
	string name;
};
class sertest_WorldObject{
	sertest_World@ world;
	
	string name;
};

sertest_Host@ sertest_host;



void serializer_test_1(){//sets values to global variables
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
	circulators.back().name = "name is correct";
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
	
	{
		coc.my_vec_int.push_back(1);
		coc.my_vec_int.push_back(2);
		coc.my_vec_int.push_back(11);
		coc.my_vec_int.push_back(22);
		
		coc.my_map_int_int.insert(2,20);
		coc.my_map_int_int.insert(3,30);
		coc.my_map_int_int.insert(1,10);
		coc.my_map_int_int.insert(4,40);
		
		coc.my_uoset_int.insert(5);
		coc.my_uoset_int.insert(3);
		coc.my_uoset_int.insert(1);
		coc.my_uoset_int.insert(2);
		
		cocs.push_back(coc);
		@cocs.back().coc_friend = @coc;
		cocs.push_back(coc);
		@cocs.back().coc_friend = @coc;
	}
	
	
	@sertest_host = sertest_Host();
	{
		sertest_World@ world = sertest_World();
		sertest_host.worlds.push_back(world);
		
		@world.host = @sertest_host;
		world.name = "real world";
		
		{
			sertest_WorldObject@ wob = sertest_WorldObject();
			world.objects.push_back(wob);
			wob.name = "wob 1";
		}
		{
			sertest_WorldObject@ wob = sertest_WorldObject();
			world.objects.push_back(wob);
			@world.host = @sertest_host;
			wob.name = "wob 2";
		}
	}
	{
		sertest_World@ world = sertest_World();
		sertest_host.worlds.push_back(world);
		
		@world.host = @sertest_host;
		world.name = "fantasy world";
		
		{
			sertest_WorldObject@ wob = sertest_WorldObject();
			world.objects.push_back(wob);
			wob.name = "wob 11";
		}
		{
			sertest_WorldObject@ wob = sertest_WorldObject();
			world.objects.push_back(wob);
			wob.name = "wob 22";
		}
	}
	
	
	// circulators.back().mymap.insert(25,55);
	// circulators.back().mymap.insert(45,75);
	
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
//the serializer stores all globals between these tests
void serializer_test_2(){//set global variables to empty / boring values
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
	
	circulators.back().name = "incorrect name";
	// circulators.back().mymap.clear();
	
	coc.clear();
	
	for(auto it = cocs.begin(); it++;){
		it.value.clear();
	}
	cocs.clear();
	
	
	for(auto it = sertest_host.worlds.begin(); it++;){
		for(auto it2 = it.value.objects.begin(); it2++;){
			it2.value.name = "nup";
		}
		it.value.objects.clear();
		it.value.name = "nope";
	}
	sertest_host.worlds.clear();
	@sertest_host = null;
}
//the serializer restores the state of global variables between these tests
void serializer_test_3(){//print global variable values, check if they are correct / actually exist
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
	
	Print("---test serializer circulator---");
	Print("circulators count = " + circulators.size());
	Print("circulators count circular = " + circulators.back().mylist.size());
	Print("circulator name = " + circulators.back().mylist[0].name);
	// Print("------");
	// for(auto it = circulators.back().mymap.begin(); it++;){
		// Print("val = " + it.key + " , "+ it.value);
	// }
	
	
	Print("---test serializer coc---");
	for(auto it = coc.my_vec_int.begin(); it++;){
		Print("val = " + it.value);
	}
	
	Print("---test serializer cocs---");
	for(auto it = cocs.begin(); it++;){
		Print("------");
		for(auto it2 = it.value.my_vec_int.begin(); it2++;){
			Print("val = " + it2.value);
		}
		Print("------");
		for(auto it2 = it.value.my_map_int_int.begin(); it2++;){
			Print("val = " + it2.key + " , "+ it2.value);
		}
		Print("friend size = " + it.value.coc_friend.my_vec_int.size());
	}
	
	Print("---test serializer world objects---");
	if(@sertest_host != null){
		for(auto it = sertest_host.worlds.begin(); it++;){
			Print("world name = " + it.value.name);
			for(auto it2 = it.value.objects.begin(); it2++;){
				Print("  wob name = "+it2.value.name);
			}
			Print("world host worldcount = " + it.value.host.worlds.size());
		}
	}
}