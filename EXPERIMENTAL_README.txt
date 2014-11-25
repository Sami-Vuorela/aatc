Provides easier iterator syntax

for(auto it = my_container.begin();it++;){
	Print("value = " + it.current());
}

This syntax is slightly slower during loop initialization than the old

for(vector_iterator it(@my_vector);it++;){
	Print("value = " + it.current());
}

, because of 1 additional constructor and 1 copy, angelscript might even decide that the constructor needs an allocation, even though the iterator is a value object.

Created using angelscript svn version 2076
will merge to master branch when the angelscript version following 2.29.2 is released