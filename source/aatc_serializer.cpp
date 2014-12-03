#include "aatc_config.hpp"
#if aatc_CONFIG_USE_ASADDON_SERIALIZER

#include "aatc_serializer.hpp"

#include "aatc_vector.hpp"
#include "aatc_list.hpp"
#include "aatc_set.hpp"
#include "aatc_unordered_set.hpp"

BEGIN_AS_NAMESPACE



template<typename containertype_template, typename cond_is_associative, int containertype_id> struct aatc_serializer_usertype_container_shared_1tp : public CUserType{
	//functor for adding to a container, different for associative
	template<typename containertype, typename cond_is_associative> class functor_adder{};
	template<typename containertype> class functor_adder<containertype, aatc_Y>{
		public:
			void operator()(containertype* container, void* item)const { container->insert(item); }
	};
	template<typename containertype> class functor_adder<containertype, aatc_N>{
		public:
			void operator()(containertype* container, void* item)const { container->push_back(item); }
	};

	void Store(CSerializedValue* val_root, void* ptr){
		aatc_container_base* container_base = (aatc_container_base*)ptr;

		containertype_template* container_template = dynamic_cast<containertype_template*>(container_base);

		if(container_template){
			if(container_template->handlemode){

			} else{
				for(auto it = container_template->begin(); it != container_template->end(); it++){
					val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>(*it), container_template->astypeid_content));
				}
			}
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

		containertype_template* container_template = dynamic_cast<containertype_template*>(container_base);

		if(container_template){
			container_template->Clear();

			if(container_template->handlemode){

			} else{
				const int size = val_root->m_children.size();
				for(int i = 0; i < size; i++){
					void* serialized_object = nullptr;
					serialized_object = container_base->engine->CreateScriptObject(container_template->objtype_content);
					val_root->m_children[i]->Restore(serialized_object, container_template->astypeid_content);
					//container_template->push_back(serialized_object);
					functor_adder<containertype_template, cond_is_associative> adder; adder(container_template, serialized_object);
				}
			}
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

#if aatc_CONFIG_USE_ASADDON_SERIALIZER_also_register_string_usertype
	struct aatc_serializer_usertype_string : public CUserType{
		void Store(CSerializedValue *val, void *ptr){
			val->SetUserData(new aatc_type_string(*(aatc_type_string*)ptr));
		}
		void Restore(CSerializedValue *val, void *ptr){
			aatc_type_string *buffer = (aatc_type_string*)val->GetUserData();
			*(aatc_type_string*)ptr = *buffer;
		}
		void CleanupUserData(CSerializedValue *val){
			aatc_type_string *buffer = (aatc_type_string*)val->GetUserData();
			delete buffer;
		}
	};
#endif





void aatc_register_for_serializer(asIScriptEngine* engine, CSerializer* serializer){
	char textbuf[1000];

	#if aatc_CONFIG_USE_ASADDON_SERIALIZER_also_register_string_usertype
		serializer->AddUserType(new aatc_serializer_usertype_string(), "string");
	#endif

	serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_vector_template,aatc_N, aatc_CONTAINERTYPE::VECTOR>(), aatc_name_script_container_vector);
	//serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_list_template, aatc_CONTAINERTYPE::LIST>(), aatc_name_script_container_list);
	//serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_set_template, aatc_CONTAINERTYPE::SET>(), aatc_name_script_container_set);
	//serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_unordered_set_template, aatc_CONTAINERTYPE::UNORDERED_SET>(), aatc_name_script_container_unordered_set);

	//serializer->AddUserType(new aatc_serializer_usertype_container_vector_template(), aatc_name_script_container_vector);
	
	//sprintf_s(textbuf, 1000, "%s<string>", aatc_name_script_container_vector);
	//serializer->AddUserType(new aatc_serializer_usertype_container_vector_tempspec_string(), textbuf);
	//serializer->AddUserType(new aatc_serializer_usertype_container_vector_tempspec_string(), "vector<int>");
}

END_AS_NAMESPACE
#endif