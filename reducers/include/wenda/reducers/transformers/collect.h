#ifndef WENDA_REDUCERS_TRANSFORMERS_COLLECT_H_INCLUDED
#define WENDA_REDUCERS_TRANSFORMERS_COLLECT_H_INCLUDED

/**
* @file collect.h
* This file implements the collect() transformer for reducibles.
*/

#include "../reducers_common.h"

#include <utility>
#include <type_traits>

#include "../reduce.h"
#include "../fold.h"

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
    template<typename Reducer, typename Expander>
	struct collect_reducing_function
	{
		Reducer const& reducer;
		Expander const& expander;

		collect_reducing_function(Reducer const& reducer, Expander const& expander)
			: reducer(reducer), expander(expander)
		{
		}

        template<typename Value, typename Seed>
		typename std::decay<Seed>::type operator()(Seed&& seed, Value&& value) const
		{
			return expander(std::forward<Value>(value)) 
				   | reduce(reducer, std::forward<Seed>(seed));
		}
	};

    template<typename ExpandFunction>
	struct collect_reducible_expression
	{
		ExpandFunction expandFunction;

		collect_reducible_expression(ExpandFunction expandFunction)
			: expandFunction(std::move(expandFunction))
		{
		}
	};
}

/**
* This class implements a reducible that corresponds to
* a source reducible that has been mapped through the expand function
* and subsequently flattened.
*/
template<typename Reducible, typename ExpandFunction>
struct collect_reducible
{
	Reducible reducible;
	ExpandFunction expandFunction;

	collect_reducible(Reducible reducible, ExpandFunction expandFunction)
		: reducible(std::move(reducible)), expandFunction(std::move(expandFunction))
	{
	}
};

/**
* Overload the reduce() function for references to @ref collect_reducible.
*/
template<typename Reducible, typename ExpandFunction, typename Reducer, typename Seed>
typename std::decay<Seed>::type reduce(collect_reducible<Reducible, ExpandFunction> const& reducible, Reducer&& reducer, Seed&& seed)
{
	typedef detail::collect_reducing_function<typename std::decay<Reducer>::type, ExpandFunction> collect_reducer_t;
	return reduce(
		reducible.reducible, 
		collect_reducer_t(std::forward<Reducer>(reducer), 
		reducible.expandFunction), std::forward<Seed>(seed));
}

/**
* Overload the reduce() function for r-value reference to @ref collect_reducible.
*/
template<typename Reducible, typename ExpandFunction, typename Reducer, typename Seed>
typename std::decay<Seed>::type reduce(collect_reducible<Reducible, ExpandFunction>&& reducible, Reducer&& reducer, Seed&& seed)
{
	typedef detail::collect_reducing_function<typename std::decay<Reducer>::type, ExpandFunction> collect_reducer_t;
	return reduce(
		std::move(reducible.reducible),
		collect_reducer_t(std::forward<Reducer>(reducer), std::move(reducible.expandFunction)), 
		std::forward<Seed>(seed));
}

/**
* Overload the fold() function for references to @ref collect_reducible.
*/
template<typename Foldable, typename ExpandFunction, typename Reduce, typename Combine>
typename detail::fold_return_type<collect_reducible<Foldable, ExpandFunction>, Reduce, Combine>::type
fold(collect_reducible<Foldable, ExpandFunction> const& foldable, Reduce&& reduce, Combine&& combine)
{
	typedef detail::collect_reducing_function<typename std::decay<Reduce>::type, ExpandFunction> collect_reducer_t;
	return fold(
		foldable.reducible,
		collect_reducer_t(std::forward<Reduce>(reduce), foldable.expandFunction), 
		std::forward<Combine>(combine));
}

/**
* Overload the fold() function for r-value references to @ref collect_reducible.
*/
template<typename Foldable, typename ExpandFunction, typename Reduce, typename Combine>
typename detail::fold_return_type<collect_reducible<Foldable, ExpandFunction>, Reduce, Combine>::type
fold(collect_reducible<Foldable, ExpandFunction>&& foldable, Reduce&& reduce, Combine&& combine)
{
	typedef detail::collect_reducing_function<typename std::decay<Reduce>::type, ExpandFunction> collect_reducer_t;
	return fold(
		std::move(foldable.reducible),
		collect_reducer_t(std::forward<Reduce>(reduce), std::move(foldable.expandFunction)), 
		std::forward<Combine>(combine));
}

namespace detail
{
    template<typename Reducible, typename ExpandFunction>
    collect_reducible<typename std::decay<Reducible>::type, ExpandFunction>
	operator|(Reducible&& reducible, collect_reducible_expression<ExpandFunction> const& expr)
	{
		typedef collect_reducible<typename std::decay<Reducible>::type, ExpandFunction> return_t;
		return return_t(std::forward<Reducible>(reducible), expr.expandFunction);
	}

    template<typename Reducible, typename ExpandFunction>
    collect_reducible<typename std::decay<Reducible>::type, ExpandFunction>
	operator|(Reducible&& reducible, collect_reducible_expression<ExpandFunction>&& expr)
	{
		typedef collect_reducible<typename std::decay<Reducible>::type, ExpandFunction> return_t;
		return return_t(std::forward<Reducible>(reducible), std::move(expr.expandFunction));
	}
}

/**
* Creates a new reducible that corresponds to the source reducible,
* with each element expanded to a reducible by the expansion function,
* and subsequently flattened.
* It corresponds to SelectMany in C#, or the monadic bind in sequence monads.
* @param reducible The reducible to collect
* @param expandFunction The expansion function. It must have signature (Value) -> Reducible.
* @returns A reducible object implementing the indicated behaviour.
*/
template<typename Reducible, typename ExpandFunction>
collect_reducible<typename std::decay<Reducible>::type, typename std::decay<ExpandFunction>::type>
collect(Reducible&& reducible, ExpandFunction&& expandFunction)
{
	typedef collect_reducible<typename std::decay<Reducible>::type, typename std::decay<ExpandFunction>::type> return_type;
	return return_type(std::forward<Reducible>(reducible), std::forward<ExpandFunction>(expandFunction));
}

/**
* Expands and flattens the given reducible with the given expansion function.
* This is similar to the two-argument version, except that the reducible should
* be passed in by pipeing.
* @param expandFunction The expansion function. It must have signature (Value) -> Reducible.
*/
template<typename ExpandFunction>
detail::collect_reducible_expression<typename std::decay<ExpandFunction>::type>
collect(ExpandFunction&& expandFunction)
{
	typedef detail::collect_reducible_expression<typename std::decay<ExpandFunction>::type> return_t;
	return return_t(std::forward<ExpandFunction>(expandFunction));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_TRANSFORMERS_COLLECT_H_INCLUDED