#ifndef __definedh_asexttest_as_basics_engine
#define __definedh_asexttest_as_basics_engine

#include <stdio.h>
#include <iostream>
#include <string>
#include "angelscript.h"



void aet_Print(const std::string& a);
void aet_PrintTabIncrement();
void aet_PrintTabDecrement();



BEGIN_AS_NAMESPACE

asIScriptEngine* aet_CreateEngine();

void aet_RuntimeExceptionCallback(asIScriptContext* ctx, void* obj);

void test_Interop(asIScriptEngine* engine, asIScriptModule* module);
void test_Interop_register(asIScriptEngine* engine);

END_AS_NAMESPACE




#endif