#ifndef WENDA_REDUCERS_FOLD_H_INCLUDED
#define WENDA_REDUCERS_FOLD_H_INCLUDED

/**
* @file fold.h
* This file contains a generic implementation of the fold() function.
* fold() is similar to reduce(), but must operate over a monoid, which
* enables opportunities to parallelise the operation.
*/

#include "reducers_common.h"

#include <utility>
#include <type_traits>

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
    template<typename T, typename ReduceFunction, typename CombineFunction>
	class has_foldable_member_function
	{
		template<typename U> static std::true_type test(
			typename std::add_pointer<decltype(std::declval<U>().fold(std::declval<ReduceFunction>(), std::declval<CombineFunction>()))>::type);
		template<typename U> static std::false_type test(...);
	public:
		typedef decltype(test<T>(nullptr)) type;
		static const bool value = type::value;
	};
}

/**
* Folds the given @p foldable using the given reduction function and combination function.
* The reduction function is to be applied to subranges of the @p foldable, and must be of signature
* compatible with (Seed, Value) -> Seed for a type Seed and the type Value of the elements in the @p foldable.
* The combine function should be an associative function of signature compatible with (Seed, Seed) -> Seed.
* It should also be overloaded with a nullary function call operator, that when invoked, returns identity value.
* @param foldable The foldable object to be folded.
* @param reduce The reduction function. It must be an invokable object with a signature compatible with (Seed, Value) -> Seed.
* @param combine The combination function. It must be an overloaded invokable type, invokable as a nullary function, which returns
* the identity value for the two argument operator. It must also be invokable as a binary function, with signature compatible with
* (Seed, Seed) -> Seed.
* @returns The result of the aggregation over the given foldable.
*/
template<typename Foldable, typename ReduceFunction, typename CombineFunction>
typename std::enable_if<
	detail::has_foldable_member_function<Foldable, ReduceFunction, CombineFunction>::value,
	typename std::decay<typename std::result_of<CombineFunction()>::type>::type
>::type
fold(Foldable&& foldable, ReduceFunction&& reduce, CombineFunction&& combine)
{
	return foldable.fold(std::forward<ReduceFunction>(reduce), std::forward<CombineFunction>(combine));
}

namespace detail
{
	template<typename ReduceFunction, typename CombineFunction>
	struct fold_expression
	{
		CombineFunction combine;
		ReduceFunction reduce;

		fold_expression(ReduceFunction reduce, CombineFunction combine)
			: combine(std::move(combine)), reduce(std::move(reduce))
		{
		}
	};

	template<typename Foldable, typename ReduceFunction, typename CombineFunction>
	typename std::decay<typename std::result_of<CombineFunction()>::type>::type
	operator|(Foldable&& foldable, fold_expression<ReduceFunction, CombineFunction> const& expr)
	{
		return fold(std::forward<Foldable>(foldable), expr.function, expr.element);
	}

    template<typename Foldable, typename ReduceFunction, typename CombineFunction>
    typename std::decay<typename std::result_of<CombineFunction()>::type>::type
	operator|(Foldable&& foldable, fold_expression<ReduceFunction, CombineFunction>&& expr)
	{
		return fold(std::forward<Foldable>(foldable), std::move(expr.combine), std::move(expr.reduce));
	}
}

/**
* Two argument version of fold() that is used with the pipe expressions.
* @sa fold()
*/
template<typename ReduceFunction, typename CombineFunction>
detail::fold_expression<typename std::decay<CombineFunction>::type, typename std::decay<ReduceFunction>::type>
fold(ReduceFunction&& reduce, CombineFunction&& combine)
{
	typedef detail::fold_expression<typename std::decay<CombineFunction>::type, typename std::decay<ReduceFunction>::type> return_t;
	return return_t(reduce, combine);
}

WENDA_REDUCERS_NAMESPACE_END

#endif