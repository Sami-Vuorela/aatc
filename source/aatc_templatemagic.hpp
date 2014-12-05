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


#ifndef _includedh_aatc_templatemagic
#define _includedh_aatc_templatemagic


#include <tuple>


/*
	Runtime iterator.
	Used by AATC to iterate over all the available container types

	Iterates over the int numbers iteration_start ... iteration_end and calls 'functor_operate' with 1 template parameter, the current iteration
	Iterating 2...4 will call the functor 3 times at functor_operate<2>(), functor_operate<3>(), functor_operate<4>()
	Iterating 0...0 will call the functor once at functor_operate<0>()
*/
template<int iteration_start, int iteration_end, template<int> class functor_operate> class aatc_tm_iterator_functor{
public:
	void operator()()const{
		aatc_tm_iterator_functor_actual<iteration_start, iteration_end, iteration_end - iteration_start, functor_operate> f; f();
	}
};


template<int iteration_start, int iteration_end, int iteration_current, template<int> class functor_operate> class aatc_tm_iterator_functor_actual{
public:
	void operator()()const{
		functor_operate<iteration_end - iteration_current> f_operate; f_operate();
		aatc_tm_iterator_functor_actual<iteration_start, iteration_end, iteration_current - 1, functor_operate> f; f();
	}
};
template<int iteration_start, int iteration_end, template<int> class functor_operate> class aatc_tm_iterator_functor_actual<iteration_start, iteration_end, -1, functor_operate>{
public:
	void operator()()const{}
};


/*
	Same as the basic iterator but this thing forwards an argument to the functor, use a tuple for multiple arguments
*/
template<int iteration_start, int iteration_end, template<int, typename> class functor_operate, typename tup_t> class aatc_tm_iterator_1arg_functor{
public:
	void operator()(tup_t& inputs)const{
		aatc_tm_iterator_1arg_functor_actual<iteration_start, iteration_end, iteration_end - iteration_start, functor_operate, tup_t> f; f((inputs));
	}
};


template<int iteration_start, int iteration_end, int iteration_current, template<int, typename> class functor_operate, typename tup_t> class aatc_tm_iterator_1arg_functor_actual{
public:
	void operator()(tup_t& inputs)const{
		functor_operate<iteration_end - iteration_current, tup_t> f_operate; f_operate((inputs));
		aatc_tm_iterator_1arg_functor_actual<iteration_start, iteration_end, iteration_current - 1, functor_operate, tup_t> f; f((inputs));
	}
};
template<int iteration_start, int iteration_end, template<int, typename> class functor_operate, typename tup_t> class aatc_tm_iterator_1arg_functor_actual<iteration_start, iteration_end, -1, functor_operate, tup_t>{
public:
	void operator()(tup_t& inputs)const{}
};



#endif