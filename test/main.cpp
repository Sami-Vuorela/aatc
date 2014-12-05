#include "as_engine.hpp"
#include "scriptbuilder\scriptbuilder.h"

#include "serializer\serializer.h"

#include "../source/aatc_common.hpp"




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

	asIScriptFunction* func_scriptmain = module->GetFunctionByName("scriptmain");

	asIScriptContext* cc = engine->RequestContext();

		cc->Prepare(func_scriptmain);
		cc->Execute();

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