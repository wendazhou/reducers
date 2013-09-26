#ifndef WENDA_REDUCIBLE_ITERATOR_PAIR_REDUCIBLE_H_INCLUDED
#define WENDA_REDUCIBLE_ITERATOR_PAIR_REDUCIBLE_H_INCLUDED

#include "reducers_common.h"

#include <utility>
#include <type_traits>

WENDA_REDUCERS_NAMESPACE_BEGIN

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
	Seed reduce(Function const& function, Seed seed) const
	{
		auto it = start;

		while (it != end)
		{
			seed = function(*start, std::move(seed));
			++it;
		}

		return seed;
	}
};

template<typename It1, typename It2>
iterator_pair_reducible<typename std::decay<It1>::type> 
make_iterator_pair_reducible(It1&& start, It2&& end)
{
	static_assert(std::is_same<typename std::decay<It1>::type, typename std::decay<It2>::type>::value, "start and end must be iterators of the same type");
	return iterator_pair_reducible<typename std::decay<It1>::type>(std::forward<It1>(start), std::forward<It2>(end));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCIBLE_ITERATOR_PAIR_REDUCIBLE_H_INCLUDED
