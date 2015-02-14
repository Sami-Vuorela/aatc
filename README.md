New features in the experimental branch:
----------



auto iterators with container::begin()
----------

Provides easier iterator syntax  
  
for(auto it = my_container.begin();it++;){  
	Print("value = " + it.current());  
}  
  
This syntax is slightly slower during loop initialization than the old  
  
for(vector_iterator it(@my_vector);it++;){  
	Print("value = " + it.current());  
}  
  
, because of 1 additional constructor and 1 copy, angelscript might even decide that the constructor needs an allocation, even though the iterator is a value object.  
Ofcourse that doesn't matter much if your containers contain a lot of objects.  
  
If you don't like "begin" as the function name, you can change it in the config at the following line  
#define aatc_name_script_container_method_begin "begin"  
  
Created using angelscript svn version 2076  



virtual property accessors
----------

You can now access iterators without calling functions with the new convenient virtual property accessors.  
Their names can be configured in the config.  
  
Syntax:  
for(auto it = myvec.begin();it++;){  
  MyPrint("current value = " + it.value);  
}  



serializer support
----------

Provides support for the as addon serializer.  

Syntax:  
	CSerializer* my_serializer = new CSerializer();  
	aatc_serializer_register(engine, my_serializer);  
  
	my_serializer->Store(my_module);  
  
	//Do_Things_Requiring_Script_Serialization()  
  
	my_serializer->Restore(my_module);  
	aatc_serializer_cleanup(engine, my_serializer);  
  
	delete my_serializer;  


All the new template majicks required for serializer support have conjured up a new nightmare:  
MSVC2013 gave me this error: "fatal error C1128: number of sections exceeded object file format limit : compile with /bigobj" when compiling the registration cpp files.  
Apparently all the templates are using more than 65536 sections in the .obj file and thats too much for MSVC2013 default settings. Google "C1128" for help for setting that "/bigobj" setting.  
This problem showed up only in debug mode.  
More help here: http://stackoverflow.com/questions/16596876/object-file-has-too-many-sections  
  
This problem went away by separating the registration process to multiple cpp files.  
