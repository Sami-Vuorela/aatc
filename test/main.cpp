#include "as_engine.hpp"






void main(){
	//AngelScript::asPrepareMultithread();

	as::asIScriptEngine* engine = as::aet_CreateEngine();

	{
		as::CScriptBuilder builder;
		builder.StartNewModule(engine, "testmodule");
		builder.AddSectionFromFile("script/main.as");
		builder.BuildModule();
	}

	as::asIScriptModule* module = engine->GetModule("testmodule");

	as::asIScriptFunction* func_scriptmain = module->GetFunctionByName("scriptmain");

	//as::asIScriptContext* cc = engine->CreateContext();
	//cc->SetExceptionCallback(as::asFUNCTION(as::aet_RuntimeExceptionCallback), NULL, as::asCALL_CDECL);

	as::asIScriptContext* cc = engine->RequestContext();

	for(int i = 0; i < 1; i++){
		cc->Prepare(func_scriptmain);
		cc->Execute();

		engine->GarbageCollect(as::asGC_FULL_CYCLE);
		engine->GarbageCollect(as::asGC_FULL_CYCLE | as::asGC_DESTROY_GARBAGE);
	}

	//cc->Release();
	engine->ReturnContext(cc);

	engine->GarbageCollect(as::asGC_FULL_CYCLE);
	engine->GarbageCollect(as::asGC_FULL_CYCLE | as::asGC_DESTROY_GARBAGE);

	engine->Release();


	std::cout << "\nTesting finished, press enter to close console.";
	std::cin.get();
}