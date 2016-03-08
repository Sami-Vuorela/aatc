#include "as_engine.hpp"
#include "scriptbuilder\scriptbuilder.h"

#include "serializer\serializer.h"

#include "../source/aatc.hpp"




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



	asIScriptFunction* func_scriptmain = module->GetFunctionByName("scriptmain");

	asIScriptContext* cc = engine->RequestContext();

		cc->Prepare(func_scriptmain);
		cc->Execute();

		#if TEST_SERIALIZER
				{
					cc->Prepare(module->GetFunctionByName("serializer_test_1")); cc->Execute();


					CSerializer backup;
					aatc::serializer::Register(engine, &backup);
					backup.Store(module);

					cc->Prepare(module->GetFunctionByName("serializer_test_2")); cc->Execute();

					backup.Restore(module);
					aatc::serializer::Cleanup(engine, &backup);

					cc->Prepare(module->GetFunctionByName("serializer_test_3")); cc->Execute();
				}
		#endif


	engine->ReturnContext(cc);

	test_Interop(engine, module);

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