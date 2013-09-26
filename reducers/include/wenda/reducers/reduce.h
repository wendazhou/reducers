#ifndef WENDA_REDUCERS_REDUCE_H_INCLUDED
#define WENDA_REDUCERS_REDUCE_H_INCLUDED

#include "reducers_common.h"

#include <utility>
#include <type_traits>

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
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
}

template<typename Reducible, typename FuncType, typename SeedType>
SeedType reduce(Reducible&& reducible, FuncType&& function, SeedType&& seed)
{
	return reducible.reduce(std::forward<FuncType>(function), std::forward<SeedType>(seed));
}

template<typename FuncType, typename Seed>
detail::reduce_expression<typename std::decay<FuncType>::type, typename std::decay<Seed>::type>
reduce(FuncType&& functype, Seed&& seed)
{
	typedef detail::reduce_expression<typename std::decay<FuncType>::type, typename std::decay<Seed>::type> return_t;
	return return_t(std::forward<FuncType>(functype), std::forward<Seed>(seed));
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