#ifndef WENDA_REDUCERS_REDUCE_H_INCLUDED
#define WENDA_REDUCERS_REDUCE_H_INCLUDED

#include "reducers_common.h"

#include <utility>

WENDA_REDUCERS_NAMESPACE_BEGIN

template<typename Reducible, typename FuncType, typename SeedType>
SeedType reduce(Reducible&& reducible, FuncType&& function, SeedType&& seed)
{
	return reducible.reduce(std::forward<FuncType>(function), std::forward<SeedType>(seed));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_REDUCE_H_INCLUDED