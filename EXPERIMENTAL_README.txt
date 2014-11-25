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
Requires a newer version of angelscript than the "stable" 2.29.2 because its got some bugs.

Will merge to master branch and do documentation when the angelscript version following 2.29.2 is released.