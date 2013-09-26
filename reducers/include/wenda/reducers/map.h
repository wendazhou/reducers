#ifndef WENDA_REDUCERS_MAP_H_INCLUDED
#define WENDA_REDUCERS_MAP_H_INCLUDED

#include "reducers_common.h"

#include <utility>
#include <type_traits>

#include "reduce.h"

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
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
*/
template<typename MapFunction, typename Reducible>
class map_reducible
{
	Reducible reducible;
	MapFunction mapFunction;
public:
	map_reducible(MapFunction mapFunction, Reducible reducible)
		: reducible(std::move(reducible)), mapFunction(std::move(mapFunction))
	{
	}

    template<typename Reducer, typename Seed>
	Seed reduce(Reducer&& reducer, Seed&& seed) const
	{
		typedef detail::map_reducing_function<MapFunction, typename std::decay<Reducer>::type> map_function_type;
		return WENDA_REDUCERS_NAMESPACE::reduce(reducible, map_function_type(mapFunction, std::forward<Reducer>(reducer)), std::forward<Seed>(seed));
	}
};

namespace detail
{
	/**
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

	template<typename Reducible, typename MapFunction>
	map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type>
	operator|(Reducible&& reducible, map_reducible_expression<MapFunction>&& mapExpression)
	{
		typedef map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type> return_t;
		return return_t(std::move(mapExpression.mapFunction), std::forward<Reducible>(reducible));
	}

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
* @param reducible The reducible that is to have its value mapped.
* @param mapFunction A function that maps the elements of the given reducible.
* @returns A new reducible that implements the described semantics when reduced.
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
*/
template<typename MapFunction>
detail::map_reducible_expression<typename std::decay<MapFunction>::type>
map(MapFunction&& mapFunction)
{
	typedef detail::map_reducible_expression<typename std::decay<MapFunction>::type> return_t;
	return return_t(std::forward<MapFunction>(mapFunction));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_MAP_H_INCLUDED