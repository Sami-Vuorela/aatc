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


#ifndef _includedh_aatc_container_vector
#define _includedh_aatc_container_vector



#include "aatc_common.hpp"
#include "aatc_container_shared_template.hpp"



BEGIN_AS_NAMESPACE
namespace aatc {
	namespace container {



		class vector : public aatc::container::shared::templated::Containerbase<aatc_acit_vector<void*>, aatc_CONTAINERTYPE::VECTOR> {
		public:
			typename typedef aatc::container::shared::templated::Containerbase < aatc_acit_vector<void*>, aatc_CONTAINERTYPE::VECTOR> containerbase;



			vector(asIObjectType* objtype);
			vector(const vector& other);
			~vector();

			static vector* Factory(asIObjectType* objtype);
			static vector* Factory_copy(asIObjectType* objtype, const vector& other);

			vector& operator=(const vector& other);
			vector& Swap(vector& other);
		};



	};//namespace container
};//namespace aatc
END_AS_NAMESPACE



#endif