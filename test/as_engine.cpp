#include "as_engine.hpp"

#include "scriptstdstring/scriptstdstring.h"

#include "../source/aatc.hpp"

//#include "scriptarray/scriptarray.h"









void aet_Print(const std::string& a){
	std::cout << a << "\n";
}



BEGIN_AS_NAMESPACE

void aet_MessageCallback(const asSMessageInfo* msg, void* param){
	char textbuf[2000];
	sprintf_s(textbuf, 2000, "SCRIPT ERROR :: %s , row %i , type=%i , msg=%s", msg->section, msg->row, msg->type, msg->message);
	aet_Print(textbuf);
}
void aet_RuntimeExceptionCallback(asIScriptContext* ctx, void* obj){
	char textbuf[2000];
	sprintf_s(textbuf, 2000, "SCRIPT RUNTIME EXCEPTION :: %s :: %s ::at line %i :: %s", ctx->GetExceptionFunction()->GetModuleName(), ctx->GetExceptionFunction()->GetDeclaration(), ctx->GetExceptionLineNumber(), ctx->GetExceptionString());
	aet_Print(textbuf);
}


asIScriptContext* aet_engine_contextcallback_request(asIScriptEngine* engine, void* userdata){
	asIScriptContext* cc = engine->CreateContext();

	cc->SetExceptionCallback(asFUNCTION(aet_RuntimeExceptionCallback), NULL, asCALL_CDECL);

	return cc;
}
void aet_engine_contextcallback_return(asIScriptEngine* engine, asIScriptContext* context, void* userdata){
	context->Release();
}



asIScriptEngine* aet_CreateEngine(){
	asIScriptEngine* engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

	engine->SetMessageCallback(asFUNCTION(aet_MessageCallback), nullptr, asCALL_CDECL);

	engine->SetContextCallbacks(aet_engine_contextcallback_request, aet_engine_contextcallback_return);



	//RegisterScriptArray(engine, 0);
	//RegisterScriptHandle(engine);
	RegisterStdString(engine);
	//RegisterStdStringUtils(engine);

	//RegisterScriptArray(engine, 0);


	aatc::RegisterAllContainers(engine);



	int r = 0;
	r = engine->RegisterGlobalFunction("void Print(string &in)", asFUNCTION(aet_Print), asCALL_CDECL); assert(r >= 0);



	return engine;
}



END_AS_NAMESPACE