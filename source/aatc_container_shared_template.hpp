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


#ifndef _includedh_aatc_shared_1t_generic
#define _includedh_aatc_shared_1t_generic



#include "aatc_common.hpp"
#include "aatc_container_shared.hpp"

//#include "cm/core/print.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {
		namespace shared {
			namespace templated {



				//doxygen skip
				#ifndef DOXYGEN_SHOULD_SKIP_THIS

				//we need a wrapper around the base container so that we can
				//feed the constructor a fancy functor to handle internal functions that require script functions to be called
				namespace base_container_wrapper {
					template<class T_container> class Basic : public T_container {
					public:
						Basic(asIScriptEngine* engine, containerfunctor_Settings* settings) {}
					};

					template<class T_container, class T_param1> class Singleparam : public T_container {
					public:
						Singleparam(asIScriptEngine* _engine, containerfunctor_Settings* settings) :
							T_container(T_param1(_engine, settings))
						{}
					};

					template<class T_container, class T_param1, class T_param2, std::size_t default_bucket_count> class Hashed : public T_container {
					public:
						Hashed(asIScriptEngine* _engine, containerfunctor_Settings* settings) :
							T_container(default_bucket_count, T_param1(_engine, settings), T_param2(_engine, settings))
						{}
					};
				};

				#endif
				//doxygen skip



				/*!\brief Internal template monster

				*/
				template<class T_container, int T_CONTAINERTYPEID, class bcw = base_container_wrapper::Basic<T_container>> class aatc_container_shared_1tp_template : public aatc_container_base,
				public bcw,
				public aatc_refcounted_GC,
				public containerfunctor_Settings
				{
				public:
					typename typedef T_container::iterator iteratortype;

					//typename typedef std::conditional<NEED_COMPHELPER, aatc_bcw_comphelper<T_container>, aatc_bcw_basic<T_container>>::type bct;
					//typename typedef bcw bct;

					asIObjectType* objtype_container;
					asIObjectType* objtype_content;
					aatc_type_astypeid astypeid_container;
					aatc_type_astypeid astypeid_content;
					asDWORD typeflags_container;
					asDWORD typeflags_content;

					bool handlemode_needref;

					engine_level_storage* els;

					aatc_container_operations_bitmask_type missing_functions;

					bool directcomp_forced;

					aatc_container_shared_1tp_template(asIScriptEngine* _engine, asIObjectType* _objtype) :
						bcw(_engine, this),
						aatc_refcounted_GC(),
						objtype_container(_objtype),
						directcomp_forced(0)
					{
						engine = _engine;

						handlemode_directcomp = aatc_CONFIG_DEFAULT_HANDLEMODE_DIRECTCOMP;

						objtype_content = objtype_container->GetSubType();

						astypeid_container = objtype_container->GetTypeId();
						astypeid_content = objtype_container->GetSubTypeId();

						typeflags_container = objtype_container->GetFlags();
						typeflags_content = objtype_content->GetFlags();

						//problem_nofunc_eq_or_cmp = 0;
						//problem_nofunc_cmp = 0;

						els = aatc_Get_ELS(engine);
						aatc_containertype_specific_storage* ctss = els->GetContainerTypeSpecificStorage(T_CONTAINERTYPEID);

						if (astypeid_content & asTYPEID_OBJHANDLE) {
							handlemode = 1;
							handlemode_needref = 1;
							if (typeflags_content & asOBJ_NOCOUNT) {
								handlemode_needref = 0;
							}
						} else {
							handlemode = 0;
							handlemode_directcomp = 0;
						}

						aatc_template_specific_storage* tss = ctss->GetTemplateSpecificStorage(astypeid_content);
						func_equals = tss->func_equals;
						func_cmp = tss->func_cmp;
						func_hash = tss->func_hash;

						missing_functions = tss->missing_functions;

				#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE
						if (missing_functions) {
							bool bad = 1;

				#if aatc_CONFIG_ENABLE_ERRORCHECK_RUNTIME_MISSINGFUNCTIONS_ZERO_TOLERANCE_USE_DIRECTCOMP
							if (handlemode) {//force handlemode directcomp if anything is missing
								handlemode_directcomp = 1;
								directcomp_forced = 1;
								bad = 0;
							}
				#endif

							if (bad) {
								char msg[1000];
								sprintf_s(msg, 1000, "Type '%s' has missing methods required for container '%s'.", objtype_content->GetName(), objtype_container->GetName());
								asGetActiveContext()->SetException(msg);
							}
						}
				#endif

						engine->NotifyGarbageCollectorOfNewObject(this, objtype_container);
					}
				};



			};//namespace templated
		};//namespace shared
	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif