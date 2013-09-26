#ifndef WENDA_REDUCERS_SEQUENCE_REDUCBILE_H_INCLUDED
#define WENDA_REDUCERS_SEQUENCE_REDUCBILE_H_INCLUDED

#include "reducers_common.h"

#include <utility>
#include <type_traits>

WENDA_REDUCERS_NAMESPACE_BEGIN

template<typename ElementType, typename OffsetType>
class sequence_reducible
{
	ElementType start;
	ElementType end;
	OffsetType offset;
public:
	sequence_reducible(ElementType start, ElementType end, OffsetType offset)
		: start(std::move(start)), end(std::move(end)), offset(std::move(offset))
	{
	}

    template<typename Function, typename Seed>
	Seed reduce(Function&& function, Seed seed) const
	{
		auto val = start;

		while (val < end)
		{
			seed = function(val, std::move(seed));
			val = val + offset;
		}

		return seed;
	}
};

template<typename Elem1, typename Elem2, typename OffsetType>
sequence_reducible<typename std::decay<Elem1>::type, typename std::decay<OffsetType>::type>
make_sequence_reducible(Elem1&& start, Elem2&& end, OffsetType&& offset)
{
	typedef sequence_reducible<typename std::decay<Elem1>::type, typename std::decay<OffsetType>::type> return_t;
	return return_t(std::forward<Elem1>(start), std::forward<Elem2>(end), std::forward<OffsetType>(offset));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_SEQUENCE_REDUCBILE_H_INCLUDED