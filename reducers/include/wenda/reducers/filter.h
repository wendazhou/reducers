#ifndef WENDA_REDUCERS_FILTER_H_INCLUDED
#define WENDA_REDUCERS_FILTER_H_INCLUDED

#include "reducers_common.h"

#include <utility>
#include <type_traits>

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
    template<typename Predicate, typename Reducer>
	struct filter_reducible_function
	{
		Predicate const& predicate;
		Reducer const& reducer;

		filter_reducible_function(Predicate const& predicate, Reducer const& reducer)
			: predicate(predicate), reducer(reducer)
		{
		}

        template<typename Seed, typename Value>
		Seed operator ()(Value&& value, Seed&& seed) const
		{
			if (predicate(value))
			{
				return reducer(std::forward<Value>(value), std::forward<Seed>(seed));
			}
			else
			{
				return std::forward<Seed>(seed);
			}
		}
	};

    template<typename Predicate, typename Reducer>
    filter_reducible_function<typename std::decay<Predicate>::type, typename std::decay<Reducer>::type>
	make_filter_reducible_function(Predicate&& predicate, Reducer&& reducer)
	{
		return filter_reducible_function<typename std::decay<Predicate>::type, typename std::decay<Reducer>::type>(
			std::forward<Predicate>(predicate), std::forward<Reducer>(reducer));
	}
}

template<typename Reducible, typename Predicate>
class filter_reducible
{
	Reducible reducible;
	Predicate predicate;
public:
	filter_reducible(Reducible reducible, Predicate predicate)
		: reducible(std::move(reducible)), predicate(std::move(predicate))
	{
	}

    template<typename FunctionType, typename SeedType>
	SeedType reduce(FunctionType&& function, SeedType&& seed) const
	{
		return reducible.reduce(
			detail::make_filter_reducible_function(predicate, std::forward<FunctionType>(function)), 
			std::forward<SeedType>(seed));
	}
};

template<typename Reducible, typename Predicate>
filter_reducible<typename std::decay<Reducible>::type, typename std::decay<Predicate>::type> 
filter(Reducible&& reducible, Predicate&& predicate)
{
	typedef filter_reducible<typename std::decay<Reducible>::type, typename std::decay<Predicate>::type> return_type;
	return return_type(std::forward<Reducible>(reducible), std::forward<Predicate>(predicate));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_FILTER_H_INCLUDED