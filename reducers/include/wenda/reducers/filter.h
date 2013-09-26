#ifndef WENDA_REDUCERS_FILTER_H_INCLUDED
#define WENDA_REDUCERS_FILTER_H_INCLUDED

#include "reducers_common.h"

#include <utility>
#include <type_traits>

#include "reduce.h"

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
		return WENDA_REDUCERS_NAMESPACE::reduce(
			reducible,
			detail::make_filter_reducible_function(predicate, std::forward<FunctionType>(function)), 
			std::forward<SeedType>(seed));
	}
};

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

template<typename Reducible, typename Predicate>
filter_reducible<typename std::decay<Reducible>::type, typename std::decay<Predicate>::type> 
filter(Reducible&& reducible, Predicate&& predicate)
{
	typedef filter_reducible<typename std::decay<Reducible>::type, typename std::decay<Predicate>::type> return_type;
	return return_type(std::forward<Reducible>(reducible), std::forward<Predicate>(predicate));
}

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

#endif // WENDA_REDUCERS_FILTER_H_INCLUDED