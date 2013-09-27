#ifndef WENDA_REDUCERS_REDUCE_H_INCLUDED
#define WENDA_REDUCERS_REDUCE_H_INCLUDED

/**
* @file reduce.h
* This file contains the basic implementation for a generic reduce()
* function that forwards to a type's reduce member function, along with
* an overload for a two argument reduce() to be used in piped expressions.
*/

#include "reducers_common.h"

#include <utility>
#include <type_traits>

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
	/**
    * This struct implements a holder for the parameters to reduce.
    * It is used to enable pipeing expressions through the framework.
	*/
    template<typename FuncType, typename Seed>
	struct reduce_expression
	{
		FuncType function;
		Seed seed;

		reduce_expression(FuncType function, Seed seed)
			: function(std::move(function)), seed(std::move(seed))
		{
		}
	};

	/**
    * This class implements a trait to determine whether a given type
    * is reducible with the given function type and seed type.
	*/
	template<typename T, typename FuncType, typename SeedType>
	class has_reducible_member_function
	{
		template<typename U> static std::true_type test(
			typename std::add_pointer<decltype(
			    std::declval<U>().reduce(
			        std::declval<FuncType>(), 
			        std::declval<SeedType>())
			)>::type);
		template<typename U> static std::false_type test(...);

	public:
		typedef decltype(test<T>(nullptr)) type;
		static const bool value = type::value;
	};
}

/**
* Reduces the given reducible using the given aggregate function and seed value.
* This is the default implementation of the reduce function, which works for all
* classes that implement a public reduce member function. Overload this function
* to enable reduction on other classes.
* @param reducible The reducible to reduce. It must implement a reduce member function.
* @param function The reduction/aggregation function, it must have signature (Seed, Value) -> Seed
* @param seed The initial value to be passed to the reduction function.
* @returns The result of the aggregation.
*/
template<typename Reducible, typename FuncType, typename SeedType>
typename std::enable_if<
	detail::has_reducible_member_function<
	    typename std::decay<Reducible>::type, 
	    typename std::decay<FuncType>::type, 
	    typename std::decay<SeedType>::type>::value, 
	SeedType
>::type
reduce(Reducible&& reducible, FuncType&& function, SeedType&& seed)
{
	return reducible.reduce(std::forward<FuncType>(function), std::forward<SeedType>(seed));
}

/**
* Reduces the given reducible passed in using the pipe expression
* with the given function and seed.
* This is intended to be used in pipe expressions, for example
* @code
* reducible | reduce(std::plus<int>(), 0);
* @endcode
* @param function The reduction/aggregation function. It must have signature (Value, Seed) -> Seed
* @param seed The initial value to be passed to the reduction function.
* @returns An object that can be or-ed with a reducible to reduce it.
*/
template<typename FuncType, typename Seed>
detail::reduce_expression<typename std::decay<FuncType>::type, typename std::decay<Seed>::type>
reduce(FuncType&& function, Seed&& seed)
{
	typedef detail::reduce_expression<typename std::decay<FuncType>::type, typename std::decay<Seed>::type> return_t;
	return return_t(std::forward<FuncType>(function), std::forward<Seed>(seed));
}

namespace detail
{
    template<typename Reducible, typename FuncType, typename Seed>
	Seed operator|(Reducible&& reducible, reduce_expression<FuncType, Seed> const& expr)
	{
		return reduce(std::forward<Reducible>(reducible), expr.function, expr.seed);
	}

    template<typename Reducible, typename FuncType, typename Seed>
	Seed operator|(Reducible&& reducible, reduce_expression<FuncType, Seed>&& expr)
	{
		return reduce(std::forward<Reducible>(reducible), std::move(expr.function), std::move(expr.seed));
	}
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_REDUCE_H_INCLUDED