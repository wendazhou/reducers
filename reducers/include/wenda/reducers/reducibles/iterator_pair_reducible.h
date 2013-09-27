#ifndef WENDA_REDUCERS_REDUCIBLES_ITERATOR_PAIR_REDUCIBLE_H_INCLUDED
#define WENDA_REDUCERS_REDUCIBLES_ITERATOR_PAIR_REDUCIBLE_H_INCLUDED

/**
* @file iterator_pair_reducible.h
* This file contains an implementation of a reducible
* from a pair of iterators that point to the beginning and
* the end of a range.
*/

#include "../reducers_common.h"

#include <utility>
#include <type_traits>
#include <numeric>

WENDA_REDUCERS_NAMESPACE_BEGIN

/**
* This class implements a reducible from an iterator pair.
*/
template<typename iterator_type>
class iterator_pair_reducible
{
	iterator_type start;
	iterator_type end;
public:
	iterator_pair_reducible(iterator_type start, iterator_type end)
		: start(std::move(start)), end(std::move(end))
	{}

    template<typename Function, typename Seed>
	Seed reduce(Function&& function, Seed&& seed) const
	{
		return std::accumulate(start, end, std::forward<Seed>(seed), std::forward<Function>(function));
	}
};

/**
* Creates a new reducible from an iterator pair.
* @param start An iterator to the start of the range.
* @param end An iterator to the end of the range.
* @returns A reducible that, when reduced, accumulates over the given range.
*/
template<typename It1, typename It2>
iterator_pair_reducible<typename std::decay<It1>::type> 
make_iterator_pair_reducible(It1&& start, It2&& end)
{
	static_assert(std::is_same<typename std::decay<It1>::type, typename std::decay<It2>::type>::value, "start and end must be iterators of the same type");
	return iterator_pair_reducible<typename std::decay<It1>::type>(std::forward<It1>(start), std::forward<It2>(end));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_REDUCIBLES_ITERATOR_PAIR_REDUCIBLE_H_INCLUDED
