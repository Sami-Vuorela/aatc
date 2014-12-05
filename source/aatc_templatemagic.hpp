#ifndef _includedh_aatc_templatemagic
#define _includedh_aatc_templatemagic


#include <tuple>


/*
	Compile time iterator.
	Iterates over the int numbers iteration_start ... iteration_end and calls 'functor_operate' with 1 argument, the current iteration
	Iterating 2...4 will call the functor 3 times at 2,3,4
	Iterating 0...0 will call the functor once at 0
*/
template<int iteration_start, int iteration_end, template<int> class functor_operate> class aatc_tm_iterator_functor{
public:
	void operator()(){
		aatc_tm_iterator_functor_actual<iteration_start, iteration_end, iteration_end - iteration_start, functor_operate> f; f();
	}
};


template<int iteration_start, int iteration_end, int iteration_current, template<int> class functor_operate> class aatc_tm_iterator_functor_actual{
public:
	void operator()(){
		functor_operate<iteration_end - iteration_current> f_operate; f_operate();
		aatc_tm_iterator_functor_actual<iteration_start, iteration_end, iteration_current - 1, functor_operate> f; f();
	}
};
template<int iteration_start, int iteration_end, template<int> class functor_operate> class aatc_tm_iterator_functor_actual<iteration_start, iteration_end, -1, functor_operate>{
public:
	void operator()(){}
};


/*
	Same as the basic iterator but this thing forwards an argument to the functor, use a tuple for multiple arguments
*/
template<int iteration_start, int iteration_end, template<int, typename> class functor_operate, typename tup_t> class aatc_tm_iterator_1arg_functor{
public:
	void operator()(tup_t&& inputs){
		aatc_tm_iterator_1arg_functor_actual<iteration_start, iteration_end, iteration_end - iteration_start, functor_operate, tup_t> f; f(std::forward<tup_t>(inputs));
	}
};


template<int iteration_start, int iteration_end, int iteration_current, template<int, typename> class functor_operate, typename tup_t> class aatc_tm_iterator_1arg_functor_actual{
public:
	void operator()(tup_t&& inputs){
		functor_operate<iteration_end - iteration_current, tup_t> f_operate; f_operate(std::forward<tup_t>(inputs));
		aatc_tm_iterator_1arg_functor_actual<iteration_start, iteration_end, iteration_current - 1, functor_operate, tup_t> f; f(std::forward<tup_t>(inputs));
	}
};
template<int iteration_start, int iteration_end, template<int, typename> class functor_operate, typename tup_t> class aatc_tm_iterator_1arg_functor_actual<iteration_start, iteration_end, -1, functor_operate, tup_t>{
public:
	void operator()(tup_t&& inputs){}
};



#endif