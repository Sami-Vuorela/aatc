New features in the experimental branch:
----------



auto iterators with container::begin()
----------

Provides easier iterator syntax  

```
for(auto it = my_container.begin();it++;){
	Print("value = " + it.current());
}
```
  
This syntax is slightly slower during loop initialization than the old  

```
for(vector_iterator it(@my_vector);it++;){
	Print("value = " + it.current());
}
```
  
, because of 1 additional constructor and 1 copy, angelscript might even decide that the constructor needs an allocation, even though the iterator is a value object.  
Ofcourse that doesn't matter much if your containers contain a lot of objects.  
  
If you don't like "begin" as the function name, you can change it in the config at the following line
```
#define aatc_name_script_container_method_begin "begin"
```
  
Created using angelscript svn version 2076  



virtual property accessors
----------

You can now access iterators without calling functions with the new convenient virtual property accessors.  
Their names can be configured in the config.  
  
Syntax:
```
for(auto it = myvec.begin();it++;){
  MyPrint("current value = " + it.value);
}
```



serializer support
----------

Provides support for the as addon serializer.  

Syntax:
```
	CSerializer* my_serializer = new CSerializer();
	aatc_serializer_register(engine, my_serializer);

	my_serializer->Store(my_module);

	//Do_Things_Requiring_Script_Serialization()

	my_serializer->Restore(my_module);
	aatc_serializer_cleanup(engine, my_serializer);

	delete my_serializer;
```

All the new template majicks required for serializer support have conjured up a new nightmare:  
MSVC2013 gave me this error: "fatal error C1128: number of sections exceeded object file format limit : compile with /bigobj" when compiling the registration cpp files.  
Apparently all the templates are using more than 65536 sections in the .obj file and thats too much for MSVC2013 default settings. Google "C1128" for help for setting that "/bigobj" setting.  
This problem showed up only in debug mode.  
More help here: http://stackoverflow.com/questions/16596876/object-file-has-too-many-sections  
  
This problem went away by separating the registration process to multiple cpp files.  




operator[] for maps
----------

map and unordered_map containers now have an operator[].
It can be used to access existing elements by key, or to add new things.

Example of adding new things and looking up existing things.
```
map<int,int> mymap;

mymap[1] = 11;
mymap[3] = 33;
mymap[2] = 22;

Print("value at 1 = " + mymap[1]);
Print("value at 2 = " + mymap[2]);
Print("value at 3 = " + mymap[3]);
```


new container operations using iterators
----------

[Forum post](http://www.gamedev.net/topic/661910-template-containers-angelscript-addon-library-release/page-2#entry5204434)  

New methods:
```
iterator container::end() //returns an iterator that points at the end, useful for...
bool iterator::operator==(const iterator &in) //checking if your iterator equals end
bool iterator::IsEnd() //check it with 1 c++ function call and no need for creating another iterator object

bool container::erase(const iterator &in) //erase at iterator, returns true if something was erased (if iterator was not end)
size_t container::erase(const iterator_range_begin &in,const iterator_range_end &in) //erase a range, returns amount of things erased

//non map containers
iterator container::find_iterator(const value &in) //find that returns an iterator, returns end if not found

//vector and list
void container::insert(const iterator &in, const value &in) //insert before iterator, using end iterator will insert at the end

//map / uo_map
iterator container::find_iterator(const key &in) //find that returns an iterator, returns end if not found
```


iterator safety with version numbers
----------

[Forum post](http://www.gamedev.net/topic/661910-template-containers-angelscript-addon-library-release/page-2#entry5206539)  

Every operation that changes a container increments a version number.  
If you try to access an iterator and it's version number differs from the container's, an exception will be thrown.
Without this, illegal access will crash.  
This obviously reduces runtime performance.  

Enable / disable in the config at this line:
```
#define aatc_CONFIG_ENABLE_ERRORCHECK_ITERATOR_SAFETY_VERSION_NUMBERS 1
```
  
Added one new method for iterators, always returns true if safety is disabled.
```
bool iterator::IsValid()
```
