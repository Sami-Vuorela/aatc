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



/*
	This file has been cut in half to reduce the amount of "object file sections".
	Who knows what those are, apparently they are created by includes and templates. MSVC2013 gave some error about them and a solution ("/bigobj" command line switch).
	Other compilers might have even more trouble with this, so I cut it in half to possibly support other compilers better.
*/



#include "aatc_tempspecs.hpp"





BEGIN_AS_NAMESPACE



void aatc_Initializer::RegisterTempspecs2(){
	#if aatc_include_primitive_native_tempspec_UINT32
		aatc_reghelp_tempspec_all<aatc_type_uint32, 1, 1, 1>(engine, "uint");
	#endif
	#if aatc_include_primitive_native_tempspec_UINT64
		aatc_reghelp_tempspec_all<aatc_type_uint64, 1, 1, 1>(engine, "uint64");
	#endif

	#if aatc_include_primitive_native_tempspec_FLOAT32
		aatc_reghelp_tempspec_all<aatc_type_float32, 1, 1, 1>(engine, "float");
	#endif
	#if aatc_include_primitive_native_tempspec_FLOAT64
		aatc_reghelp_tempspec_all<aatc_type_float64, 1, 1, 1>(engine, "double");
	#endif

	#if aatc_include_primitive_native_tempspec_STRING
		aatc_reghelp_tempspec_all<aatc_type_string, 1, 1, 1>(engine, "string");
	#endif
}



END_AS_NAMESPACE