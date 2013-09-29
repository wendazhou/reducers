#ifndef WENDA_REDUCERS_TRANSFORMERS_FILTER_H_INCLUDED
#define WENDA_REDUCERS_TRANSFORMERS_FILTER_H_INCLUDED

/**
* @file filter.h
* This file implements the filter() reducible transformer.
*/

#include "../reducers_common.h"

#include <utility>
#include <type_traits>

#include "../reduce.h"
#include "../fold.h"

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
    template<typename Predicate, typename Reducer>
	struct filter_reducible_function
	{
		Predicate predicate;
		Reducer reducer;

		filter_reducible_function(Predicate predicate, Reducer reducer)
			: predicate(std::move(predicate)), reducer(std::move(reducer))
		{
		}

        template<typename Seed, typename Value>
		typename std::decay<Seed>::type operator ()(Seed&& seed, Value&& value) const
		{
			if (predicate(value))
			{
				return reducer(std::forward<Value>(seed), std::forward<Seed>(value));
			}
			else
			{
				return std::forward<Seed>(seed);
			}
		}
	};
}

/**
* This class implements a reducible that, when reduced
* reduces the original reducible with its elements filtered
* by a predicate.
*/
template<typename Reducible, typename Predicate>
struct filter_reducible
{
	Reducible reducible;
	Predicate predicate;

	filter_reducible(Reducible reducible, Predicate predicate)
		: reducible(std::move(reducible)), predicate(std::move(predicate))
	{
	}
};

/**
* Overloads the reduce() function to reduce reducibles of type @ref filter_reducible.
*/
template<typename Reducible, typename Predicate, typename Reducer, typename Seed>
typename std::decay<Seed>::type reduce(filter_reducible<Reducible, Predicate> const& reducible, Reducer&& reducer, Seed&& seed)
{
	typedef detail::filter_reducible_function<Predicate, typename std::decay<Reducer>::type> filter_reducer_t;
	return reduce(
		reducible.reducible,
		filter_reducer_t(reducible.predicate, std::forward<Reducer>(reducer)),
		std::forward<Seed>(seed));
}

/**
* Overloads the reduce() function to reduce r-value references to reducibles of type @ref filter_reducible.
*/
template<typename Reducible, typename Predicate, typename Reducer, typename Seed>
typename std::decay<Seed>::type reduce(filter_reducible<Reducible, Predicate>&& reducible, Reducer&& reducer, Seed&& seed)
{
	typedef detail::filter_reducible_function<Predicate, typename std::decay<Reducer>::type> filter_reducer_t;
	return reduce(
		std::move(reducible.reducible),
		filter_reducer_t(std::move(reducible.predicate), std::forward<Reducer>(reducer)),
		std::forward<Seed>(seed));
}

/**
* Overloads the fold() function to fold @ref filter_reducible
*/
template<typename Foldable, typename Predicate, typename Reduce, typename Combine>
typename detail::fold_return_type<filter_reducible<Foldable, Predicate>, Reduce, Combine>::type
fold(filter_reducible<Foldable, Predicate> const& foldable, Reduce&& reduce, Combine&& combine)
{
	typedef detail::filter_reducible_function<Predicate, typename std::decay<Reduce>::type> filter_fold_t;

	return fold(
		foldable.reducible,
		filter_fold_t(foldable.predicate, std::forward<Reduce>(reduce)),
		std::forward<Combine>(combine));
}

/**
* Overloads the fold() function to fold r-value references to @ref filter_reducible
*/
template<typename Foldable, typename Predicate, typename Reduce, typename Combine>
typename detail::fold_return_type<filter_reducible<Foldable, Predicate>, Reduce, Combine>::type
fold(filter_reducible<Foldable, Predicate>&& foldable, Reduce&& reduce, Combine&& combine)
{
	typedef detail::filter_reducible_function<Predicate, typename std::decay<Reduce>::type> filter_fold_t;

	return fold(
		std::move(foldable.reducible),
		filter_fold_t(std::move(foldable.predicate), std::forward<Reduce>(reduce)),
		std::forward<Combine>(combine));
}

namespace detail
{
	template<typename Predicate>
	struct filter_reducible_expression
	{
		Predicate predicate;

		filter_reducible_expression(Predicate predicate)
			: predicate(std::move(predicate))
		{
		}
    };
}

/**
* Creates a new reducible that when reduced, reduces the original reducible
* and only keeps elements corresponding to the given predicate.
* @param reducible The original reducible to filter.
* @param predicate The filtering predicate. It must be a function of signature (Value) -> (convertible-to-bool).
* @returns A new reducible that implements the filtering reduce behaviour.
*/
template<typename Reducible, typename Predicate>
filter_reducible<typename std::decay<Reducible>::type, typename std::decay<Predicate>::type> 
filter(Reducible&& reducible, Predicate&& predicate)
{
	typedef filter_reducible<typename std::decay<Reducible>::type, typename std::decay<Predicate>::type> return_type;
	return return_type(std::forward<Reducible>(reducible), std::forward<Predicate>(predicate));
}

/**
* Filters the given reducible.
* This function is similar to the two-argument version, but should have
* the reducible passed in by pipeing.
* @param predicate The filtering predicate. It must be a function of signature (Value) -> (convertible-to-bool)
* @returns A implementation helper object that enables pipeing.
*/
template<typename Predicate>
detail::filter_reducible_expression<typename std::decay<Predicate>::type>
filter(Predicate&& predicate)
{
	typedef detail::filter_reducible_expression<typename std::decay<Predicate>::type> return_t;
	return return_t(std::forward<Predicate>(predicate));
}

namespace detail
{
    template<typename Reducible, typename Predicate>
    filter_reducible<typename std::decay<Reducible>::type, Predicate>
	operator|(Reducible&& reducible, filter_reducible_expression<Predicate> const& expr)
	{
		typedef filter_reducible<typename std::decay<Reducible>::type, Predicate> return_t;
		return return_t(std::forward<Reducible>(reducible), expr.predicate);
	}

    template<typename Reducible, typename Predicate>
    filter_reducible<typename std::decay<Reducible>::type, Predicate>
	operator|(Reducible&& reducible, filter_reducible_expression<Predicate>&& expr)
	{
		typedef filter_reducible<typename std::decay<Reducible>::type, Predicate> return_t;
		return return_t(std::forward<Reducible>(reducible), std::move(expr.predicate));
	}
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_TRANSFORMERS_FILTER_H_INCLUDED