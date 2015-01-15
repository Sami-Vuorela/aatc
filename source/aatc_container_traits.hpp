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



#ifndef _includedh_aatc_container_traits
#define _includedh_aatc_container_traits

#include "aatc_common.hpp"



BEGIN_AS_NAMESPACE

class aatc_container_traits_1tp_base{
public:
	typedef aatc_N trait_is_associative;

	typedef aatc_N trait_needfunc_BACK_WRITE;
	typedef aatc_N trait_needfunc_BACK_READ;
	typedef aatc_N trait_needfunc_FRONT_WRITE;
	typedef aatc_N trait_needfunc_FRONT_READ;

	typedef aatc_N trait_needfunc_INSERT;
	typedef aatc_N trait_needfunc_INSERT_GENERIC_INDEX;

	typedef aatc_N trait_needfunc_ERASE_POSITION;
	typedef aatc_N trait_needfunc_ERASE_VALUE;
	typedef aatc_N trait_needfunc_ERASE_GENERIC_INDEX;
	typedef aatc_N trait_needfunc_ERASE_GENERIC_VALUE;

	typedef aatc_N trait_needfunc_CONTAINS_NATIVE;

	typedef aatc_N trait_needfunc_OP_INDEX;

	typedef aatc_N trait_needfunc_RESERVE;

	typedef aatc_N trait_needfunc_SORT_NATIVE;
	typedef aatc_N trait_needfunc_SORT_GENERIC;



	typedef aatc_Y trait_iterator_editable;
	typedef aatc_N trait_iterator_const;

	typedef aatc_N trait_needfunc_ERASE_ITERATOR;
	typedef aatc_N trait_needfunc_ERASE_RANGE_ITERATOR;
	typedef aatc_N trait_needfunc_FIND_ITERATOR;

	typedef aatc_N trait_needfunc_INSERT_ITERATOR;
};



class aatc_container_traits_vector : public aatc_container_traits_1tp_base{
public:
	typedef aatc_Y trait_needfunc_BACK_WRITE;
	typedef aatc_Y trait_needfunc_BACK_READ;
	typedef aatc_Y trait_needfunc_FRONT_READ;

	typedef aatc_Y trait_needfunc_INSERT_GENERIC_INDEX;

	typedef aatc_Y trait_needfunc_ERASE_POSITION;
	typedef aatc_Y trait_needfunc_ERASE_GENERIC_INDEX;
	typedef aatc_Y trait_needfunc_ERASE_GENERIC_VALUE;

	typedef aatc_Y trait_needfunc_OP_INDEX;

	typedef aatc_Y trait_needfunc_RESERVE;

	typedef aatc_Y trait_needfunc_SORT_GENERIC;

	typedef aatc_Y trait_needfunc_ERASE_ITERATOR;
	typedef aatc_Y trait_needfunc_ERASE_RANGE_ITERATOR;

	typedef aatc_Y trait_needfunc_INSERT_ITERATOR;
};

class aatc_container_traits_list : public aatc_container_traits_1tp_base{
public:
	typedef aatc_Y trait_needfunc_BACK_WRITE;
	typedef aatc_Y trait_needfunc_BACK_READ;
	typedef aatc_Y trait_needfunc_FRONT_WRITE;
	typedef aatc_Y trait_needfunc_FRONT_READ;

	typedef aatc_Y trait_needfunc_ERASE_GENERIC_VALUE;

	typedef aatc_Y trait_needfunc_SORT_NATIVE;

	typedef aatc_Y trait_needfunc_ERASE_ITERATOR;
	typedef aatc_Y trait_needfunc_ERASE_RANGE_ITERATOR;

	typedef aatc_Y trait_needfunc_INSERT_ITERATOR;
};

class aatc_container_traits_set : public aatc_container_traits_1tp_base{
public:
	typedef aatc_Y trait_is_associative;

	typedef aatc_Y trait_needfunc_INSERT;
	typedef aatc_Y trait_needfunc_ERASE_VALUE;
	typedef aatc_Y trait_needfunc_CONTAINS_NATIVE;



	typedef aatc_N trait_iterator_editable;
	typedef aatc_Y trait_iterator_const;

	typedef aatc_Y trait_needfunc_ERASE_ITERATOR;
	typedef aatc_Y trait_needfunc_ERASE_RANGE_ITERATOR;

	typedef aatc_Y trait_needfunc_FIND_ITERATOR;
};

class aatc_container_traits_unordered_set : public aatc_container_traits_1tp_base{
public:
	typedef aatc_Y trait_is_associative;

	typedef aatc_Y trait_needfunc_INSERT;
	typedef aatc_Y trait_needfunc_ERASE_VALUE;
	typedef aatc_Y trait_needfunc_CONTAINS_NATIVE;



	typedef aatc_N trait_iterator_editable;
	typedef aatc_Y trait_iterator_const;

	typedef aatc_Y trait_needfunc_ERASE_ITERATOR;
	typedef aatc_Y trait_needfunc_ERASE_RANGE_ITERATOR;

	typedef aatc_Y trait_needfunc_FIND_ITERATOR;
};


END_AS_NAMESPACE





#endif