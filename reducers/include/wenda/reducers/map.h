#ifndef WENDA_REDUCERS_MAP_H_INCLUDED
#define WENDA_REDUCERS_MAP_H_INCLUDED

#include "reducers_common.h"

#include <utility>
#include <type_traits>

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
		Seed operator()(Value&& value, Seed&& seed) const
		{
			return reducer(mapFunction(std::forward<Value>(value)), std::forward<Seed>(seed));
		}
	};
}

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
		return reducible.reduce(map_function_type(mapFunction, std::forward<Reducer>(reducer)), std::forward<Seed>(seed));
	}
};

template<typename MapFunction, typename Reducible>
map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type>
map(Reducible&& reducible, MapFunction&& mapFunction)
{
	typedef map_reducible<typename std::decay<MapFunction>::type, typename std::decay<Reducible>::type> return_type;
	return return_type(std::forward<MapFunction>(mapFunction), std::forward<Reducible>(reducible));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_MAP_H_INCLUDED