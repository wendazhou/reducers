#ifndef WENDA_REDUCERS_TRANSFORMERS_MAP_H_INCLUDED
#define WENDA_REDUCERS_TRANSFORMERS_MAP_H_INCLUDED

/**
* @file map.h
* This file implements the map() reducible transformer.
*/

#include "../reducers_common.h"

#include <utility>
#include <type_traits>

#include "../reduce.h"

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
	/**
    * @internal
    * This struct implements the functor type used when reducing
    * a reducible as transformed by a map() transformer.
	*/
    template<typename MapFunction, typename Reducer>
	struct map_reducing_function
	{
		MapFunction const& mapFunction;
		Reducer const& reducer;

		map_reducing_function(MapFunction const& mapFunction, Reducer const& reducer)
			: reducer(reducer), mapFunction(mapFunction)
		{
		}

        template<typename Value, typename Seed>
		typename std::decay<Seed>::type operator()(Seed&& seed, Value&& value) const
		{
			return reducer(std::forward<Seed>(seed), mapFunction(std::forward<Value>(value)));
		}
	};
}

/**
* This class implements a reducible that, when reduced,
* reduces the source reducible as if its elements where mapped
* by a function.
* @tparam MapFunction The type of the mapping functor. It must be invokable.
* @tparam Reducible The type of the reducible that is mapped. It must model a reducible.
*/
template<typename MapFunction, typename Reducible>
struct map_reducible
{
	Reducible reducible; ///< the reducible that is mapped.
	MapFunction mapFunction; ///< the mapping function.

	map_reducible(MapFunction mapFunction, Reducible reducible)
		: reducible(std::move(reducible)), mapFunction(std::move(mapFunction))
	{
	}
};

/**
* Overloads the reduce() function to reduce reducibles of type @ref map_reducible.
* @param reducible The instance of @ref map_reducible to be reduced.
* @param reducer A reducing function.
* @param seed The seed for the reduction operation.
*/
template<typename Reducible, typename MapFunction, typename Reducer, typename Seed>
typename std::decay<Seed>::type reduce(map_reducible<MapFunction, Reducible> const& reducible, Reducer&& reducer, Seed&& seed)
{
	typedef detail::map_reducing_function<MapFunction, typename std::decay<Reducer>::type> map_function_type;
	return reduce(
        reducible.reducible,
		map_function_type(reducible.mapFunction, std::forward<Reducer>(reducer)),
		std::forward<Seed>(seed));
}

/**
* Overloads the reduce() function to reduce r-value references to reducibles of type @ref map_reducible.
* @param reducible The instance of @ref map_reducible to be reduced.
* @param reducer A reducing function.
* @param seed The seed for the reductio operation.
*/
template<typename Reducible, typename MapFunction, typename Reducer, typename Seed>
typename std::decay<Seed>::type reduce(map_reducible<MapFunction, Reducible>&& reducible, Reducer&& reducer, Seed&& seed)
{
	typedef detail::map_reducing_function<MapFunction, typename std::decay<Reducer>::type> map_function_type;
	return reduce(
        std::move(reducible.reducible),
		map_function_type(std::move(reducible.mapFunction), std::forward<Reducer>(reducer)),
		std::forward<Seed>(seed));
}

namespace detail
{
	/**
    * @internal
    * This struct is a holder for the arguments of the map function.
    * It is used to enable pipe expressions for map.
	*/
	template<typename MapFunction>
	struct map_reducible_expression
	{
		MapFunction mapFunction;

		map_reducible_expression(MapFunction const& mapFunction)
			: mapFunction(mapFunction)
		{
		}

		map_reducible_expression(MapFunction&& mapFunction)
			: mapFunction(std::move(mapFunction))
		{
		}
	};

	/**
    * @internal
    * Operator overload to enable the use of \ref map_reducible_expression in pipe expressions.
	*/
	template<typename Reducible, typename MapFunction>
	map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type>
	operator|(Reducible&& reducible, map_reducible_expression<MapFunction>&& mapExpression)
	{
		typedef map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type> return_t;
		return return_t(std::move(mapExpression.mapFunction), std::forward<Reducible>(reducible));
	}

	/**
    * @internal
    * Operator overload to enable the use of r-value references to \ref map_reducible_expression in pipe expressions.
	*/
    template<typename Reducible, typename MapFunction>
    map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type>
	operator|(Reducible&& reducible, map_reducible_expression<MapFunction> const& mapExpression)
	{
		typedef map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type> return_t;
		return return_t(mapExpression.mapFunction, std::forward<Reducible>(reducible));
	}
}

/**
* Creates a new reducible that when reduced, reduces the
* original reducible with its values mapped through the given function.
* Conceptually, this acts as if the underlying sequence were transformed by the
* mapping function, and operates lazily.
* @remark It is often preferable to use the one argument version map(MapFunction) in
* a piped expression.
* @param reducible The reducible that is to have its value mapped.
* @param mapFunction A function that maps the elements of the given reducible.
* @returns A new reducible that implements the described semantics when reduced.
* @sa map()
*/
template<typename MapFunction, typename Reducible>
map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type>
map(Reducible&& reducible, MapFunction&& mapFunction)
{
	typedef map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type> return_type;
	return return_type(std::forward<MapFunction>(mapFunction), std::forward<Reducible>(reducible));
}

/**
* Maps the given reducible through the given function.
* This is equivalent to the two-argument version, but is
* destined to be used in a pipe expression.
* @param mapFunction A function that maps the elements of the reducible to be given.
* @returns A holder object that can be composed with a reducible through a pipe | operator to perform the operation.
* @sa map()
*/
template<typename MapFunction>
detail::map_reducible_expression<typename std::decay<MapFunction>::type>
map(MapFunction&& mapFunction)
{
	typedef detail::map_reducible_expression<typename std::decay<MapFunction>::type> return_t;
	return return_t(std::forward<MapFunction>(mapFunction));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_TRANSFORMERS_MAP_H_INCLUDED