#ifndef WENDA_REDUCERS_RANGE_REDUCIBLE_H_INCLUDED
#define WENDA_REDUCERS_RANGE_REDUCIBLE_H_INCLUDED

#include "reducers_common.h"

#include <iterator>

WENDA_REDUCERS_NAMESPACE_BEGIN

template<typename Range>
class range_reducible
{
	Range const& range;

	range_reducible(Range const& range)
		: range(range)
	{}

    template<typename FunctionType, typename SeedType>
	SeedType reduce(FunctionType&& function, SeedType seed)
	{
		for (auto&& val : range)
		{
			seed = function(val, std::move(seed));
		}

		return seed;
	}
};

WENDA_REDUCERS_NAMESPACE_END

#endif WENDA_REDUCERS_RANGE_REDUCIBLE_H_INCLUDED