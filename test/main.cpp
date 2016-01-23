#include "as_engine.hpp"
#include "scriptbuilder\scriptbuilder.h"

#include "serializer\serializer.h"

#include "../source/aatc_common.hpp"

#include "../source/aatc_container_vector.hpp"
#include "../source/aatc_container_map.hpp"




#define TEST_SERIALIZER 0



BEGIN_AS_NAMESPACE



void main_contents(){
	//AngelScript::asPrepareMultithread();

	asIScriptEngine* engine = aet_CreateEngine();

	{
		CScriptBuilder builder;
		builder.StartNewModule(engine, "testmodule");
		builder.AddSectionFromFile("script/main.as");
		builder.BuildModule();
	}

	asIScriptModule* module = engine->GetModule("testmodule");

	auto ot = engine->GetObjectTypeByDecl("map<int,int>");

	aatc::container::templated::mapped::map* cont_map = new aatc::container::templated::mapped::map(ot);
	{
		{
			int key = 1;
			int value = 11;

			cont_map->insert(&key, &value);
		}
		{
			int key = 2;
			int value = 22;

			cont_map->insert(&key, &value);
		}
	}

	aatc::container::tempspec::vector<int>* testcont = new aatc::container::tempspec::vector<int>();

	testcont->push_back(11);
	testcont->push_back(22);


	asIScriptFunction* func_scriptmain = module->GetFunctionByName("scriptmain");
	asIScriptFunction* func_scriptmain2 = module->GetFunctionByName("scriptmain2");
	asIScriptFunction* func_test_container_interface_map_1 = module->GetFunctionByName("test_container_interface_map_1");

	asIScriptContext* cc = engine->RequestContext();

		cc->Prepare(func_scriptmain);
		cc->Execute();



		cc->Prepare(func_scriptmain2);
		cc->SetArgAddress(0, testcont);
		cc->Execute();

		cc->Prepare(func_test_container_interface_map_1);
		cc->SetArgAddress(0, cont_map);
		cc->Execute();

		#if TEST_SERIALIZER
				{
					cc->Prepare(module->GetFunctionByName("serializer_test_1")); cc->Execute();


					CSerializer backup;
					aatc_serializer_register(engine, &backup);
					backup.Store(module);

					cc->Prepare(module->GetFunctionByName("serializer_test_2")); cc->Execute();

					backup.Restore(module);
					aatc_serializer_cleanup(engine, &backup);

					cc->Prepare(module->GetFunctionByName("serializer_test_3")); cc->Execute();
				}
		#endif


	engine->ReturnContext(cc);

	engine->Release();

	std::cout << "\nTesting finished, press enter to close console.";
	std::cin.get();
}

END_AS_NAMESPACE



void main(){
	#if AS_USE_NAMESPACE
		AngelScript::main_contents();
	#else
		main_contents();
	#endif
}