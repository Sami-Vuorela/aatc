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


#ifndef _includedh_aatc_aatc
#define _includedh_aatc_aatc

#include "aatc_config.hpp"



BEGIN_AS_NAMESPACE


/*!\brief The initializer class used for engine specific registration settings*/
class aatc_Initializer{
public:
	asIScriptEngine* engine;

	/*!
		This array will default to all false.
	*/
	bool include_container[aatc_CONTAINERTYPE::aatc_CONTAINERTYPE_COUNT];

	aatc_Initializer(asIScriptEngine* engine);

	/*!
		Call this after setting your settings.
	*/
	void Go();
};

/*!
Just call this if you want to have every container available and dont care about engine specific settings.
*/
void aatc_RegisterAllContainers(asIScriptEngine* engine);



END_AS_NAMESPACE
#endif