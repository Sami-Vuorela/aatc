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


#include "aatc_config.hpp"
#if aatc_CONFIG_USE_ASADDON_SERIALIZER

#include "aatc_serializer.hpp"

#include "aatc_templatemagic.hpp"
#include "aatc_internal_lists.hpp"

//#include "aatc_vector.hpp"
//#include "aatc_list.hpp"
//#include "aatc_set.hpp"
//#include "aatc_unordered_set.hpp"

BEGIN_AS_NAMESPACE

aatc_serializer_handle_storage* aatc_serializer_handle_storage_global = nullptr;



template<typename containertype, typename cond_is_associative> class aatc_serializer_store_handle_in_container{};
template<typename containertype> class aatc_serializer_store_handle_in_container<containertype, aatc_Y>{
public:
	void operator()(aatc_container_base* container, void* item)const {
		static_cast<containertype*>(container)->insert(item);
	}
};
template<typename containertype> class aatc_serializer_store_handle_in_container<containertype, aatc_N>{
public:
	void operator()(aatc_container_base* container, void* item)const {
		static_cast<containertype*>(container)->push_back(item);
	}
};

template<int i, typename TT> class aatc_serializer_store_handle_in_container_tm_iterator_init{
public:
	typename typedef std::tuple_element<i, aatc_infos_1tp_tuple>::type infotype;
	typename static const int id = infotype::container_id;
	typename typedef std::conditional<infotype::is_associative, aatc_Y, aatc_N>::type is_associative;
	typename typedef infotype::container_type_template container_type_template;

	void operator()(TT& storage)const{
		storage->funcs_store_handle[id] = aatc_serializer_store_handle_in_container<container_type_template, is_associative>();
	}
};



template<typename containertype_template, typename cond_is_associative, int containertype_id> struct aatc_serializer_usertype_container_shared_1tp : public CUserType{
	//aatc_serializer_handle_storage* storage;

	//aatc_serializer_usertype_container_shared_1tp(aatc_serializer_handle_storage* _storage):
	//	storage(_storage)
	//{}

	//functor for adding to a container, different for associative
	template<typename containertype, typename cond_is_associative> class functor_add{};
	template<typename containertype> class functor_add<containertype, aatc_Y>{
		public:
			void operator()(containertype* container, void* item)const { container->insert(item); }
	};
	template<typename containertype> class functor_add<containertype, aatc_N>{
		public:
			void operator()(containertype* container, void* item)const { container->push_back(item); }
	};

	void Store(CSerializedValue* val_root, void* ptr){
		aatc_container_base* container_base = (aatc_container_base*)ptr;

		containertype_template* container_template = dynamic_cast<containertype_template*>(container_base);

		if(container_template){
			if(container_template->handlemode){
				for(auto it = container_template->begin(); it != container_template->end(); it++){
					//const void* ptr_to_object = *it;
					//const void* ptr_to_ptr_to_object = &(const void**)ptr_to_object;
					//val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>(ptr_to_ptr_to_object), container_template->astypeid_content));

					//container_base->engine->AddRefScriptObject(*it, container_template->objtype_content);

					const void* ptr_to_object = *it;
					const void** ptr_to_ptr_to_object = &ptr_to_object;
					val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>((void*)ptr_to_ptr_to_object), container_template->astypeid_content));

					//val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>(*it), container_template->astypeid_content));
				}
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
				const std::size_t size = val_root->m_children.size();

				aatc_serializer_handle_storage_global->containers.push_back(aatc_serializer_handle_storage::Container());
				aatc_serializer_handle_storage::Container* handle_storage = &(aatc_serializer_handle_storage_global->containers.back());

				handle_storage->container = container_base;
				handle_storage->containertype_id = containertype_id;
				handle_storage->objects.assign(size, nullptr);

				for(std::size_t i = 0; i < size; i++){
					val_root->m_children[i]->Restore(&(handle_storage->objects.data()[i]), container_template->astypeid_content);
				}

				//works, but only with vector
				//container_template->assign(size, nullptr);
				//for(std::size_t i = 0; i < size; i++){
				//	val_root->m_children[i]->Restore(&(container_template->data()[i]), container_template->astypeid_content);
				//}


				//std::vector<void*> temp_handles(size,nullptr);

				//for(std::size_t i = 0; i < size; i++){
				//	void** ptr_to_ptr_to_object = &temp_handles[i];
				//	val_root->m_children[i]->Restore((void*)ptr_to_ptr_to_object, container_template->astypeid_content);
				//}

				//for(std::size_t i = 0; i < size; i++){
				//	//functor_add<containertype_template, cond_is_associative> add; add(container_template, temp_handles[i]);

				//	void* serialized_object = nullptr;
				//	void** ptr_to_ptr_to_object = &serialized_object;

				//	val_root->m_children[i]->Restore((void*)ptr_to_ptr_to_object, container_template->astypeid_content);
				//	functor_add<containertype_template, cond_is_associative> add; add(container_template, serialized_object);

				//	//void* serialized_object = nullptr;
				//	//val_root->m_children[i]->Restore((void*)serialized_object, container_template->astypeid_content);
				//	//functor_adder<containertype_template, cond_is_associative> adder; adder(container_template, *serialized_object);
				//}
			} else{
				const std::size_t size = val_root->m_children.size();
				for(std::size_t i = 0; i < size; i++){
					void* serialized_object = nullptr;
					serialized_object = container_base->engine->CreateScriptObject(container_template->objtype_content);
					val_root->m_children[i]->Restore(serialized_object, container_template->astypeid_content);
					//container_template->push_back(serialized_object);
					functor_add<containertype_template, cond_is_associative> add; add(container_template, serialized_object);
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





void aatc_serializer_register(asIScriptEngine* engine, CSerializer* serializer){
	char textbuf[1000];

	aatc_serializer_handle_storage_global = new aatc_serializer_handle_storage(engine,serializer);

	aatc_serializer_handle_storage* storage = aatc_serializer_handle_storage_global;

	//aatc_serializer_handle_storage* storage = new aatc_serializer_handle_storage;

	#if aatc_CONFIG_USE_ASADDON_SERIALIZER_also_register_string_usertype
		serializer->AddUserType(new aatc_serializer_usertype_string(), "string");
	#endif

	serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_vector_template,aatc_N, aatc_CONTAINERTYPE::VECTOR>(), aatc_name_script_container_vector);
	serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_list_template, aatc_N, aatc_CONTAINERTYPE::LIST>(), aatc_name_script_container_list);
	serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_set_template, aatc_Y, aatc_CONTAINERTYPE::SET>(), aatc_name_script_container_set);
	serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_unordered_set_template, aatc_Y, aatc_CONTAINERTYPE::UNORDERED_SET>(), aatc_name_script_container_unordered_set);

	//serializer->AddUserType(new aatc_serializer_usertype_container_vector_template(), aatc_name_script_container_vector);
	
	//sprintf_s(textbuf, 1000, "%s<string>", aatc_name_script_container_vector);
	//serializer->AddUserType(new aatc_serializer_usertype_container_vector_tempspec_string(), textbuf);
	//serializer->AddUserType(new aatc_serializer_usertype_container_vector_tempspec_string(), "vector<int>");


	aatc_tm_iterator_1arg_functor<0, aatc_infos_1tp_tuple_size - 1, aatc_serializer_store_handle_in_container_tm_iterator_init, aatc_serializer_handle_storage*> f; f(storage);
}
void aatc_serializer_cleanup(asIScriptEngine* engine, CSerializer* serializer){
	aatc_serializer_handle_storage* storage = aatc_serializer_handle_storage_global;

	if(storage){
		for(auto it = storage->containers.begin(); it != storage->containers.end(); it++){
			aatc_serializer_handle_storage::Container& cont = *it;
			for(auto it = cont.objects.begin(); it != cont.objects.end(); it++){
				storage->funcs_store_handle[cont.containertype_id](cont.container, *it);
				//aatc_serializer_store_handle_in_any_container(aatc_serializer_handle_storage_global, cont.container,cont.containertype_id, *it);
				//aatc_serializer_store_handle_in_any_container(aatc_serializer_handle_storage_global, cont.container, *it);
			}

			//aatc_container_vector_template* container_testvec = static_cast<aatc_container_vector_template*>((*it).container);
			//container_testvec->insert(container_testvec->begin(), (*it).objects.begin(), (*it).objects.end());
		}



		delete aatc_serializer_handle_storage_global;
	}
}

aatc_serializer_handle_storage::Container::Container(){}
aatc_serializer_handle_storage::Container::~Container(){}





//template<int container_id> void aatc_serializer_store_handle_in_container(aatc_serializer_handle_storage* storage, aatc_container_base* container, void* handle){}


aatc_serializer_handle_storage::aatc_serializer_handle_storage(asIScriptEngine* engine, CSerializer* serializer){}
aatc_serializer_handle_storage::~aatc_serializer_handle_storage(){}

//void aatc_serializer_store_handle_in_any_container(aatc_serializer_handle_storage* storage, aatc_container_base* container, int containertype_id, void* handle){
//	storage->funcs_store_handle[containertype_id](container, handle);
//}

END_AS_NAMESPACE
#endif