#ifndef __definedh_asexttest_as_basics_engine
#define __definedh_asexttest_as_basics_engine

#include "basics.hpp"
#include "angelscript.h"
#include "C:\Dev\VC2013\angelscript_2.29.1\add_on\scriptbuilder\scriptbuilder.h"

#if defined BEGIN_AS_NAMESPACE
namespace as = AngelScript;
#else
#define as
#endif


typedef std::string asString;


BEGIN_AS_NAMESPACE

asIScriptEngine* aet_CreateEngine();

void aet_RuntimeExceptionCallback(asIScriptContext* ctx, void* obj);


END_AS_NAMESPACE




#endif