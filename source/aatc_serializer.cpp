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

//#include "scriptarray/scriptarray.h"





BEGIN_AS_NAMESPACE

aatc_serializer_specific_storage* aatc_serializer_specific_storage_global = nullptr;



template<typename containertype, typename cond_is_associative> class aatc_serializer_store_handle_in_container_1tp{};
template<typename containertype> class aatc_serializer_store_handle_in_container_1tp<containertype, aatc_Y>{
public:
	void operator()(aatc_container_base* container, void* item)const {
		static_cast<containertype*>(container)->insert(item);
	}
};
template<typename containertype> class aatc_serializer_store_handle_in_container_1tp<containertype, aatc_N>{
public:
	void operator()(aatc_container_base* container, void* item)const {
		static_cast<containertype*>(container)->push_back(item);
	}
};

template<int i, typename TT> class aatc_serializer_store_handle_in_container_1tp_tm_iterator_init{
public:
	typename typedef std::tuple_element<i, aatc_infos_1tp_tuple>::type infotype;
	typename static const int id = infotype::container_id;
	typename typedef std::conditional<infotype::is_associative, aatc_Y, aatc_N>::type is_associative;
	typename typedef infotype::container_type_template container_type_template;

	void operator()(TT& storage)const{
		storage->funcs_store_handle_1tp[id] = aatc_serializer_store_handle_in_container_1tp<container_type_template, is_associative>();
	}
};



template<typename containertype> class aatc_serializer_store_handle_in_container_map{
public:
	void operator()(aatc_container_base* container, aatc_primunion_pair& item)const {
		static_cast<containertype*>(container)->insert(item);
	}
};

template<int i, typename TT> class aatc_serializer_store_handle_in_container_map_tm_iterator_init{
public:
	typename typedef std::tuple_element<i, aatc_infos_map_tuple>::type infotype;
	typename static const int id = infotype::container_id;
	typename typedef infotype::container_type_template container_type_template;

	void operator()(TT& storage)const{
		storage->funcs_store_handle_map[id] = aatc_serializer_store_handle_in_container_map<container_type_template>();
	}
};





template<typename containertype_template, typename cond_is_associative, int containertype_id> struct aatc_serializer_usertype_container_shared_1tp : public CUserType{
	//functor for adding to a container, different for associative containers
	template<typename containertype, typename cond_is_associative> class functor_add{};
	template<typename containertype> class functor_add<containertype, aatc_Y>{
		public:
			void operator()(containertype* container, void* item)const { container->insert(item); }
	};
	template<typename containertype> class functor_add<containertype, aatc_N>{
		public:
			void operator()(containertype* container, void* item)const { container->push_back(item); }
	};



	asIScriptEngine* engine;

	//we need a pointer to the engine
	aatc_serializer_usertype_container_shared_1tp(asIScriptEngine* _engine) :
		engine(_engine)
	{}

	void Store(CSerializedValue* val_root, void* ptr){
		aatc_container_base* container_base = (aatc_container_base*)ptr;

		containertype_template* container_template = dynamic_cast<containertype_template*>(container_base);

		if(container_template){
			if(container_template->handlemode){
				for(auto it = container_template->begin(); it != container_template->end(); it++){
					const void* ptr_to_object = *it;
					const void** ptr_to_ptr_to_object = &ptr_to_object;
					val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>((void*)ptr_to_ptr_to_object), container_template->astypeid_content));
				}
			} else{
				for(auto it = container_template->begin(); it != container_template->end(); it++){
					val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>(*it), container_template->astypeid_content));
				}
			}
		} else{
			engine_level_storage* els = aatc_Get_ELS(container_base->engine);

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

		if(!container_base->engine){
			container_base->engine = engine;
		}

		containertype_template* container_template = dynamic_cast<containertype_template*>(container_base);

		if(container_template){
			container_template->Clear();

			if(container_template->handlemode){
				const std::size_t size = val_root->m_children.size();

				aatc_serializer_specific_storage_global->containers_1tp.push_back(aatc_serializer_specific_storage::Container_1tp());
				aatc_serializer_specific_storage::Container_1tp* handle_storage = &(aatc_serializer_specific_storage_global->containers_1tp.back());

				handle_storage->container = container_base;
				handle_storage->containertype_id = containertype_id;
				handle_storage->objects.assign(size, nullptr);

				for(std::size_t i = 0; i < size; i++){
					val_root->m_children[i]->Restore(&(handle_storage->objects.data()[i]), container_template->astypeid_content);
				}
			} else{
				const std::size_t size = val_root->m_children.size();
				for(std::size_t i = 0; i < size; i++){
					void* serialized_object = nullptr;
					serialized_object = container_base->engine->CreateScriptObject(container_template->objtype_content);
					val_root->m_children[i]->Restore(serialized_object, container_template->astypeid_content);
					functor_add<containertype_template, cond_is_associative> add; add(container_template, serialized_object);
				}
			}
		} else{
			engine_level_storage* els = aatc_Get_ELS(container_base->engine);

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
			engine_level_storage* els = aatc_Get_ELS(container_base->engine);

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



template<typename containertype, int containertype_id> struct aatc_serializer_usertype_container_shared_map : public CUserType{
	void Store(CSerializedValue* val_root, void* ptr){
		aatc_container_base* container_base = (aatc_container_base*)ptr;
		containertype* container = dynamic_cast<containertype*>(container_base);

		for(auto it = container->begin(); it != container->end(); it++){
			auto& itpair = *it;

			switch(container->datahandlingid_key){
			case aatc_DATAHANDLINGTYPE::HANDLE:{
				const void* ptr_to_object = itpair.first.ptr;
				const void** ptr_to_ptr_to_object = &ptr_to_object;
				val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>((void*)ptr_to_ptr_to_object), container->astypeid_key));
			break; }
			case aatc_DATAHANDLINGTYPE::STRING:
			case aatc_DATAHANDLINGTYPE::OBJECT:{
				val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>(itpair.first.ptr), container->astypeid_key));
			break; }
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
				val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>(itpair.first.Get_Ptr_To_Primitive_Type_const(container->primitiveid_key)), container->astypeid_key));
			break; }
			};

			switch(container->datahandlingid_value){
			case aatc_DATAHANDLINGTYPE::HANDLE:{
												   const void* ptr_to_object = itpair.second.ptr;
												   const void** ptr_to_ptr_to_object = &ptr_to_object;
												   val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>((void*)ptr_to_ptr_to_object), container->astypeid_value));
												   break; }
			case aatc_DATAHANDLINGTYPE::STRING:
			case aatc_DATAHANDLINGTYPE::OBJECT:{
												   val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>(itpair.second.ptr), container->astypeid_value));
												   break; }
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
													  val_root->m_children.push_back(new CSerializedValue(val_root, "", "", const_cast<void*>(itpair.second.Get_Ptr_To_Primitive_Type_const(container->primitiveid_value)), container->astypeid_value));
													break; }
			};
		}
	}
	void Restore(CSerializedValue* val_root, void* ptr){
		aatc_container_base* container_base = (aatc_container_base*)ptr;
		containertype* container = dynamic_cast<containertype*>(container_base);

		const std::size_t size = val_root->m_children.size() / 2;

		aatc_serializer_specific_storage* storage = aatc_serializer_specific_storage_global;

		storage->containers_map.push_back(aatc_serializer_specific_storage::Container_map());
		aatc_serializer_specific_storage::Container_map& storage_container = storage->containers_map.back();

		storage_container.container = container_base;
		storage_container.containertype_id = containertype_id;
		storage_container.objects.assign(size, aatc_primunion_pair());

		for(std::size_t i = 0; i < size; i++){
			CSerializedValue* val_key = val_root->m_children[i * 2];
			CSerializedValue* val_value = val_root->m_children[i * 2 + 1];

			aatc_primunion_pair& insertpair = storage_container.objects[i];

			switch(container->datahandlingid_key){
			case aatc_DATAHANDLINGTYPE::HANDLE:{
												   val_key->Restore(&(insertpair.first.ptr), container->astypeid_key);
												   break; }
			case aatc_DATAHANDLINGTYPE::STRING:
			case aatc_DATAHANDLINGTYPE::OBJECT:{
														void* serialized_object = nullptr;
														serialized_object = container_base->engine->CreateScriptObject(container->objtype_key);
														val_key->Restore(serialized_object, container->astypeid_key);
														insertpair.first.ptr = serialized_object;
														break; }
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
													  void* ptr_to_prim = insertpair.first.Get_Ptr_To_Primitive_Type(container->primitiveid_key);
													  val_key->Restore(ptr_to_prim, container->astypeid_key);
														break; }
			};

			switch(container->datahandlingid_value){
			case aatc_DATAHANDLINGTYPE::HANDLE:{
												   val_value->Restore(&(insertpair.second.ptr), container->astypeid_value);
												   break; }
			case aatc_DATAHANDLINGTYPE::STRING:
			case aatc_DATAHANDLINGTYPE::OBJECT:{
												   void* serialized_object = nullptr;
												   serialized_object = container_base->engine->CreateScriptObject(container->objtype_value);
												   val_value->Restore(serialized_object, container->astypeid_value);
												   insertpair.second.ptr = serialized_object;
												   break; }
			case aatc_DATAHANDLINGTYPE::PRIMITIVE:{
													  void* ptr_to_prim = insertpair.second.Get_Ptr_To_Primitive_Type(container->primitiveid_value);
													  val_value->Restore(ptr_to_prim, container->astypeid_value);
													  break; }
			};
		}
	}
};

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


	//struct CArrayType : public CUserType
	//{
	//	void Store(CSerializedValue *val, void *ptr)
	//	{
	//		CScriptArray *arr = (CScriptArray*)ptr;

	//		for(unsigned int i = 0; i < arr->GetSize(); i++)
	//			val->m_children.push_back(new CSerializedValue(val, "", "", arr->At(i), arr->GetElementTypeId()));
	//	}
	//	void Restore(CSerializedValue *val, void *ptr)
	//	{
	//		CScriptArray *arr = (CScriptArray*)ptr;
	//		arr->Resize(asUINT(val->m_children.size()));

	//		for(size_t i = 0; i < val->m_children.size(); ++i)
	//			val->m_children[i]->Restore(arr->At(asUINT(i)), arr->GetElementTypeId());
	//	}
	//};


void aatc_serializer_register(asIScriptEngine* engine, CSerializer* serializer){
	char textbuf[1000];

	aatc_serializer_specific_storage_global = new aatc_serializer_specific_storage(engine,serializer);

	aatc_serializer_specific_storage* storage = aatc_serializer_specific_storage_global;

	#if aatc_CONFIG_USE_ASADDON_SERIALIZER_also_register_string_usertype
		serializer->AddUserType(new aatc_serializer_usertype_string(), "string");
	#endif

		serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_vector_template, aatc_N, aatc_CONTAINERTYPE::VECTOR>(engine), aatc_name_script_container_vector);
	serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_list_template, aatc_N, aatc_CONTAINERTYPE::LIST>(engine), aatc_name_script_container_list);
	serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_set_template, aatc_Y, aatc_CONTAINERTYPE::SET>(engine), aatc_name_script_container_set);
	serializer->AddUserType(new aatc_serializer_usertype_container_shared_1tp<aatc_container_unordered_set_template, aatc_Y, aatc_CONTAINERTYPE::UNORDERED_SET>(engine), aatc_name_script_container_unordered_set);

	serializer->AddUserType(new aatc_serializer_usertype_container_shared_map<aatc_container_map_template, aatc_CONTAINERTYPE::MAP>(), aatc_name_script_container_map);
	serializer->AddUserType(new aatc_serializer_usertype_container_shared_map<aatc_container_unordered_map_template, aatc_CONTAINERTYPE::UNORDERED_MAP>(), aatc_name_script_container_unordered_map);

	//serializer->AddUserType(new CArrayType(), "array");

	{ aatc_tm_iterator_1arg_functor<0, aatc_infos_1tp_tuple_size - 1, aatc_serializer_store_handle_in_container_1tp_tm_iterator_init, aatc_serializer_specific_storage*> f; f(storage); }
	{aatc_tm_iterator_1arg_functor<0, aatc_infos_map_tuple_size - 1, aatc_serializer_store_handle_in_container_map_tm_iterator_init, aatc_serializer_specific_storage*> f; f(storage); }
}
void aatc_serializer_cleanup(asIScriptEngine* engine, CSerializer* serializer){
	aatc_serializer_specific_storage* storage = aatc_serializer_specific_storage_global;

	if(storage){
		for(auto it = storage->containers_1tp.begin(); it != storage->containers_1tp.end(); it++){
			aatc_serializer_specific_storage::Container_1tp& cont = *it;
			for(auto it = cont.objects.begin(); it != cont.objects.end(); it++){
				storage->funcs_store_handle_1tp[cont.containertype_id](cont.container, *it);
			}
		}
		for(auto it = storage->containers_map.begin(); it != storage->containers_map.end(); it++){
			aatc_serializer_specific_storage::Container_map& cont = *it;
			for(auto it = cont.objects.begin(); it != cont.objects.end(); it++){
				storage->funcs_store_handle_map[cont.containertype_id](cont.container, *it);
			}
		}

		delete storage;
	}
}

aatc_serializer_specific_storage::Container_1tp::Container_1tp(){}
aatc_serializer_specific_storage::Container_1tp::~Container_1tp(){}
aatc_serializer_specific_storage::Container_map::Container_map(){}
aatc_serializer_specific_storage::Container_map::~Container_map(){}

aatc_serializer_specific_storage::aatc_serializer_specific_storage(asIScriptEngine* engine, CSerializer* serializer){}
aatc_serializer_specific_storage::~aatc_serializer_specific_storage(){}


END_AS_NAMESPACE
#endif