#include "as_engine.hpp"

#include "../source/aatc_container_vector.hpp"
#include "../source/aatc_container_map.hpp"

#include <iostream>



BEGIN_AS_NAMESPACE



namespace regfunc {
	namespace write {
		aatc::container::tempspec::vector<int>* vector_int_handle() {
			aatc::container::tempspec::vector<int>* result = new aatc::container::tempspec::vector<int>;

			for (int i = 0; i < 5; i++) {
				result->push_back(i);
			}

			return result;
		}

		aatc::container::mapped::templated::map* map_int_int_handle() {

			asITypeInfo* tinfo = asGetActiveContext()->GetEngine()->GetTypeInfoByDecl("map<int,int>");
			aatc::container::mapped::templated::map* result = new aatc::container::mapped::templated::map(tinfo);

			{
				int key = 1;
				int value = 11;
				result->insert(&key, &value);
			}
			{
				int key = 2;
				int value = 22;
				result->insert(&key, &value);
			}

			return result;
		}
	}
	namespace read {
		void vector_int_handle(aatc::container::tempspec::vector<int>* cont) {
			for (int v : cont->container) {
				std::cout << "  " << v << "\n";
			}
		}
		void vector_int_ref(aatc::container::tempspec::vector<int> const& cont) {
			for (int v : cont.container) {
				std::cout << "  " << v << "\n";
			}
		}
	}
}

void test_Interop_register(asIScriptEngine* engine) {
	int r = 0;

	r = engine->RegisterGlobalFunction("vector<int>@ test_interop_write_vector_int_handle()", asFUNCTION(regfunc::write::vector_int_handle), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("map<int,int>@ test_interop_write_map_int_int_handle()", asFUNCTION(regfunc::write::map_int_int_handle), asCALL_CDECL); assert(r >= 0);

	r = engine->RegisterGlobalFunction("void test_interop_read_vector_int_handle(vector<int>@)", asFUNCTION(regfunc::read::vector_int_handle), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("void test_interop_read_vector_int_ref(vector<int> &inout)", asFUNCTION(regfunc::read::vector_int_ref), asCALL_CDECL); assert(r >= 0);
}
void test_Interop(asIScriptEngine* engine, asIScriptModule* module) {
	std::cout << "\ntest_Interop\n";

	asIScriptContext* context = engine->RequestContext();

	{
		std::cout << "  test_interop_script_creates_vector_int\n";

		asIScriptFunction* func = module->GetFunctionByName("test_interop_script_creates_vector_int");
		context->Prepare(func);
		context->Execute();

		if (context->GetState() == asEContextState::asEXECUTION_FINISHED) {
			aatc::container::tempspec::vector<int>* cont = (aatc::container::tempspec::vector<int>*)context->GetReturnObject();

			std::cout << "  size = " << cont->size() << "\n";
			for (int v : cont->container) {
				std::cout << "    " << v << "\n";
			}
		} else {
			std::cout << "  error\n";
		}

		std::cout << "\n";
	}
	{
		std::cout << "  test_interop_script_creates_vector_int_handle\n";

		asIScriptFunction* func = module->GetFunctionByName("test_interop_script_creates_vector_int_handle");
		context->Prepare(func);
		context->Execute();

		if (context->GetState() == asEContextState::asEXECUTION_FINISHED) {
			aatc::container::tempspec::vector<int>* cont = (aatc::container::tempspec::vector<int>*)context->GetReturnAddress();

			std::cout << "  size = " << cont->size() << "\n";
			for (int v : cont->container) {
				std::cout << "    " << v << "\n";
			}
		} else {
			std::cout << "  error\n";
		}

		std::cout << "\n";
	}
	{
		std::cout << "  test_interop_main\n";

		asIScriptFunction* func = module->GetFunctionByName("test_interop_main");
		context->Prepare(func);
		context->Execute();
	}

	engine->ReturnContext(context);
}



END_AS_NAMESPACE