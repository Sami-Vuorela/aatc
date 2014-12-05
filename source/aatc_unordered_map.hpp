/*
The zlib/libpng License
http://opensource.org/licenses/zlib-license.php


Angelscript addon Template Containers
Copyright (c) 2014 Sami Vuorela

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1.	The origin of this software must not be misrepresented;
You must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.

2.	Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3.	This notice may not be removed or altered from any source distribution.


Sami Vuorela
samivuorela@gmail.com
*/


#ifndef _includedh_aatc_container_unordered_map
#define _includedh_aatc_container_unordered_map

#include "aatc_shared_map_template.hpp"



BEGIN_AS_NAMESPACE

typedef aatc_acit_unordered_map<
aatc_primunion,
aatc_primunion,
aatc_containerfunctor_map_hash,
aatc_containerfunctor_map_equals
> aatc_acit_unordered_map_with_functors;

/*!\brief Actual class used for templates defined in script.*/
class aatc_container_unordered_map_template : public aatc_container_shared_map_template<
	aatc_acit_unordered_map_with_functors,
	aatc_CONTAINERTYPE::UNORDERED_MAP,
	aatc_shared_map_bcw_hashmap<
	aatc_acit_unordered_map_with_functors,
	aatc_containerfunctor_map_hash,
	aatc_containerfunctor_map_equals,
	aatc_CONFIG_DEFAULT_CONTAINER_UNORDERED_MAP_DEFAULTBUCKETCOUNT
	>
>{
public:
	typedef aatc_acit_unordered_map_with_functors bt;

	aatc_container_unordered_map_template(asIScriptEngine* _engine, asIObjectType* _objtype);
	aatc_container_unordered_map_template(const aatc_container_unordered_map_template& other);
	~aatc_container_unordered_map_template();

	static aatc_container_unordered_map_template* Factory(asIObjectType* _objtype_container);
	static aatc_container_unordered_map_template* Factory_copy(asIObjectType* _objtype, const aatc_container_unordered_map_template& other);
	aatc_container_unordered_map_template& operator=(const aatc_container_unordered_map_template& other);
	aatc_container_unordered_map_template& Swap(aatc_container_unordered_map_template& other);
};

END_AS_NAMESPACE

#endif