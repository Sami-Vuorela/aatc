#include "aatc_config.hpp"
#if aatc_CONFIG_USE_ASADDON_SERIALIZER

#include "aatc_serializer.hpp"

#include "aatc_vector.hpp"

BEGIN_AS_NAMESPACE


template<int containertype_id> struct aatc_serializer_usertype_container_shared_1tp : public CUserType{
	void Store(CSerializedValue* val_root, void* ptr){
		aatc_container_base* container_base = (aatc_container_base*)ptr;

		aatc_container_vector_template* container_template = dynamic_cast<aatc_container_vector_template*>(container_base);

		if(container_template){
			container_base = nullptr;
		} else{
			aatc_engine_level_storage* els = aatc_Get_ELS(container_base->engine);

			auto itend = els->serializer_tempspec_helpers[containertype_id].end();
			for(auto it = els->serializer_tempspec_helpers[containertype_id].begin(); it != itend; it++){
				if(it->funcptr_is_thistype(container_base)){
					it->funcptr_process_store(container_base, val_root);
					break;
				}
			}
		}
	}
	void Restore(CSerializedValue* val_root, void* ptr){
		aatc_container_base* container_base = (aatc_container_base*)ptr;

		aatc_container_vector_template* container_template = dynamic_cast<aatc_container_vector_template*>(container_base);

		if(container_template){
			container_base = nullptr;
		} else{
			aatc_engine_level_storage* els = aatc_Get_ELS(container_base->engine);

			auto itend = els->serializer_tempspec_helpers[containertype_id].end();
			for(auto it = els->serializer_tempspec_helpers[containertype_id].begin(); it != itend; it++){
				if(it->funcptr_is_thistype(container_base)){
					it->funcptr_process_restore(container_base, val_root);
					break;
				}
			}
		}
	}
	void CleanupUserData(CSerializedValue* val){
		aatc_container_base* container_base = (aatc_container_base*)val->GetUserData();

		if(container_base){//templates wont have this
			aatc_engine_level_storage* els = aatc_Get_ELS(container_base->engine);

			auto itend = els->serializer_tempspec_helpers[containertype_id].end();
			for(auto it = els->serializer_tempspec_helpers[containertype_id].begin(); it != itend; it++){
				if(it->funcptr_is_thistype(container_base)){
					it->funcptr_process_cleanup(container_base, val);
					break;
				}
			}
		}
	}
};



/*
struct aatc_serializer_usertype_container_vector_template : public CUserType{
	void Store(CSerializedValue* val_root, void* ptr){
		aatc_container_base* container_base = (aatc_container_base*)ptr;

		aatc_container_vector_template* container_template = dynamic_cast<aatc_container_vector_template*>(container_base);

		if(container_template){
			container_base = nullptr;
		} else{
			aatc_engine_level_storage* els = aatc_Get_ELS(container_base->engine);

			auto itend = els->serializer_tempspec_helpers[aatc_CONTAINERTYPE::VECTOR].end();
			for(auto it = els->serializer_tempspec_helpers[aatc_CONTAINERTYPE::VECTOR].begin(); it != itend; it++){
				if(it->funcptr_is_thistype(container_base)){
					it->funcptr_process_store(container_base, val_root);
					break;
				}
			}
		}

		//aatc_container_vector_template* container = (aatc_container_vector_template*)ptr;

		//for(auto it = container->begin(); it != container->end(); it++){
		//	val_root->m_children.push_back(new CSerializedValue(val_root, "", "", *it, container->astypeid_content));
		//}
	}
	void Restore(CSerializedValue* val_root, void* ptr){
		aatc_container_base* container_base = (aatc_container_base*)ptr;

		aatc_container_vector_template* container_template = dynamic_cast<aatc_container_vector_template*>(container_base);

		if(container_template){
			container_base = nullptr;
		} else{
			aatc_engine_level_storage* els = aatc_Get_ELS(container_base->engine);

			auto itend = els->serializer_tempspec_helpers[aatc_CONTAINERTYPE::VECTOR].end();
			for(auto it = els->serializer_tempspec_helpers[aatc_CONTAINERTYPE::VECTOR].begin(); it != itend; it++){
				if(it->funcptr_is_thistype(container_base)){
					it->funcptr_process_restore(container_base, val_root);
					break;
				}
			}
		}



		//container->clear();

		//auto itend = val_root->m_children.end();
		//for(auto it = val_root->m_children.begin(); it != itend; it++){
		//	void* serialized_object = nullptr;
		//	(*it)->Restore(&serialized_object, container->astypeid_content);
		//	container->push_back(serialized_object);
		//}

		//int i = 0;
		//for(auto it = container->begin(); it != container->end(); it++){
		//	void* serialized_object = nullptr;
		//	val_root->m_children[i]->Restore(&serialized_object, container->astypeid_content);
		//	i++;
		//}
	}
	void CleanupUserData(CSerializedValue* val){
		aatc_container_base* container_base = (aatc_container_base*)val->GetUserData();

		if(container_base){//templates wont have this
			aatc_engine_level_storage* els = aatc_Get_ELS(container_base->engine);

			auto itend = els->serializer_tempspec_helpers[aatc_CONTAINERTYPE::VECTOR].end();
			for(auto it = els->serializer_tempspec_helpers[aatc_CONTAINERTYPE::VECTOR].begin(); it != itend; it++){
				if(it->funcptr_is_thistype(container_base)){
					it->funcptr_process_cleanup(container_base, val);
					break;
				}
			}
		}
	}
};
*/

struct CStringType : public CUserType{
	void Store(CSerializedValue *val, void *ptr){
		val->SetUserData(new std::string(*(std::string*)ptr));
	}
	void Restore(CSerializedValue *val, void *ptr){
		std::string *buffer = (std::string*)val->GetUserData();
		*(std::string*)ptr = *buffer;
	}
	void CleanupUserData(CSerializedValue *val){
		std::string *buffer = (std::string*)val->GetUserData();
		delete buffer;
	}
};





void aatc_register_for_serializer(asIScriptEngine* engine, CSerializer* serializer){
	char textbuf[1000];

	serializer->AddUserType(new CStringType(), "string");
	serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_CONTAINERTYPE::VECTOR>(), aatc_name_script_container_vector);
	//serializer->AddUserType(new aatc_serializer_usertype_container_vector_template(), aatc_name_script_container_vector);
	
	//sprintf_s(textbuf, 1000, "%s<string>", aatc_name_script_container_vector);
	//serializer->AddUserType(new aatc_serializer_usertype_container_vector_tempspec_string(), textbuf);
	//serializer->AddUserType(new aatc_serializer_usertype_container_vector_tempspec_string(), "vector<int>");
}

END_AS_NAMESPACE
#endif