#ifndef WENDA_REDUCERS_REDUCIBLES_SEQUENCE_REDUCIBLE_H_INCLUDED
#define WENDA_REDUCERS_REDUCIBLES_SEQUENCE_REDUCIBLE_H_INCLUDED

#include "../reducers_common.h"

#include <utility>
#include <type_traits>

WENDA_REDUCERS_NAMESPACE_BEGIN

/**
* This class implements a reducible that when corresponds to reducing over a sequence.
*/
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

	/**
    * Reduces over this sequence with the given function and seed.
	*/
    template<typename Function, typename Seed>
	Seed reduce(Function&& function, Seed seed) const
	{
		auto val = start;

		while (val < end)
		{
			seed = function(std::move(seed), val);
			val = val + offset;
		}

		return seed;
	}
};

/**
* Creates a new reducible that corresponds to the given sequence.
* The reducible acts as if it were a range from start inclusive, to the
* last element smaller than end, stepping by offset.
* start and end must be of the same type, and start + offset must be a
* valid expression of the same type as start.
* @param start The starting value of the sequence.
* @param end The upper bound of the sequence (exclusive).
* @param offset The step size
*/
template<typename Elem1, typename Elem2, typename OffsetType>
sequence_reducible<typename std::decay<Elem1>::type, typename std::decay<OffsetType>::type>
make_sequence_reducible(Elem1&& start, Elem2&& end, OffsetType&& offset)
{
	typedef sequence_reducible<typename std::decay<Elem1>::type, typename std::decay<OffsetType>::type> return_t;
	return return_t(std::forward<Elem1>(start), std::forward<Elem2>(end), std::forward<OffsetType>(offset));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_REDUCIBLES_SEQUENCE_REDUCIBLE_H_INCLUDED