#ifndef _includedh_aatc_serializer
#define _includedh_aatc_serializer

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


END_AS_NAMESPACE
#endif
#endif