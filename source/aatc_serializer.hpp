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


#ifndef _includedh_aatc_serializer
#define _includedh_aatc_serializer

#include <functional>
#include "aatc_config.hpp"
#if aatc_CONFIG_USE_ASADDON_SERIALIZER

#include "aatc.hpp"
#include "aatc_common.hpp"
#include aatc_serializer_addonpath

BEGIN_AS_NAMESPACE





template<typename T> bool aatc_serializer_containerbase_is_thistype(aatc_container_base* base){
	return dynamic_cast<T*>(base) != nullptr;
}
template<typename T> void aatc_serializer_usertype_container_shared_1tp_tempspec_store(aatc_container_base* base, CSerializedValue* val_root){
	T* container = static_cast<T*>(base);
	if(container){
		val_root->SetUserData(new T(*container));
	}
}
template<typename T> void aatc_serializer_usertype_container_shared_1tp_tempspec_restore(aatc_container_base* base, CSerializedValue* val_root){
	T* container = static_cast<T*>(base);
	if(container){
		T* buffer = (T*)val_root->GetUserData();
		*container = *buffer;
	}
}
template<typename T> void aatc_serializer_usertype_container_shared_1tp_tempspec_cleanup(aatc_container_base* base, CSerializedValue* val_root){
	T* buffer = (T*)val_root->GetUserData();
	delete buffer;
}

template<typename dt_container, int containertype_id> void aatc_serializer_register_container_shared_1tp_tempspec_helpers(aatc_engine_level_storage* els, const char* n_content){
	els->serializer_tempspec_helpers[containertype_id].push_back(aatc_engine_level_storage::serializer_helper());
	els->serializer_tempspec_helpers[containertype_id].back().funcptr_is_thistype = aatc_serializer_containerbase_is_thistype<dt_container>;
	els->serializer_tempspec_helpers[containertype_id].back().container_content_name = n_content;
	els->serializer_tempspec_helpers[containertype_id].back().funcptr_process_store = aatc_serializer_usertype_container_shared_1tp_tempspec_store<dt_container>;
	els->serializer_tempspec_helpers[containertype_id].back().funcptr_process_restore = aatc_serializer_usertype_container_shared_1tp_tempspec_restore<dt_container>;
	els->serializer_tempspec_helpers[containertype_id].back().funcptr_process_cleanup = aatc_serializer_usertype_container_shared_1tp_tempspec_cleanup<dt_container>;
}


class aatc_serializer_specific_storage{
public:
	class Container_1tp{
	public:
		aatc_container_base* container;
		int containertype_id;
		std::vector<void*> objects;

		Container_1tp();
		~Container_1tp();
	};
	class Container_map{
	public:
		aatc_container_base* container;
		int containertype_id;
		std::vector<aatc_primunion_pair> objects;

		Container_map();
		~Container_map();
	};

	//typedef void(*store_handle_in_container)(aatc_container_base* base, void* handle);
	typedef std::function<void(aatc_container_base*, void*)> store_handle_in_container_1tp;
	typedef std::function<void(aatc_container_base*, aatc_primunion_pair&)> store_handle_in_container_map;

	std::list<Container_1tp> containers_1tp;
	std::list<Container_map> containers_map;

	store_handle_in_container_1tp funcs_store_handle_1tp[aatc_CONTAINERTYPE_COUNT];
	store_handle_in_container_map funcs_store_handle_map[aatc_CONTAINERTYPE_COUNT];


	aatc_serializer_specific_storage(asIScriptEngine* engine, CSerializer* serializer);
	~aatc_serializer_specific_storage();
};
//global for now, while as addon serializer does not have serializer-object-specific userdata
extern aatc_serializer_specific_storage* aatc_serializer_specific_storage_global;

//void aatc_serializer_store_handle_in_any_container(aatc_serializer_specific_storage* storage, aatc_container_base* container,int containertype_id, void* handle);



END_AS_NAMESPACE
#endif
#endif