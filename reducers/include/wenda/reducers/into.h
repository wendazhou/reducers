#ifndef WENDA_REDUCERS_INTO_H_INCLUDED
#define WENDA_REDUCERS_INTO_H_INCLUDED

#include "reducers_common.h"

#include <utility>
#include <type_traits>

#include "reduce.h"

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
	struct output_iterator_accumulator
	{
        template<typename Iterator, typename Value>
		typename Iterator operator()(Iterator iterator, Value&& value)
		{
			*iterator = std::forward<Value>(value);
			return ++iterator;
		}
	};

    template<typename Iterator>
	struct into_expresion
	{
		Iterator iterator;

		into_expresion(Iterator iterator)
			: iterator(std::move(iterator))
		{}
	};

}

/**
* Reduces a reducible into a sequence.
* This accumulates each element of the reducible
* into a sequence that is output to the given iterator.
* @param reducible The reducible to accumulate into the sequence.
* @param iterator The output iterator to be used to write the accumulated sequence.
* @returns The iterator at the position that has been last written to.
*/
template<typename Iterator, typename Reducible>
typename std::decay<Iterator>::type
into(Reducible&& reducible, Iterator&& iterator)
{
	return reduce(std::forward<Reducible>(reducible), detail::output_iterator_accumulator(), std::forward<Iterator>(iterator));
}

namespace detail
{
    template<typename Reducible, typename Iterator>
	Iterator operator|(Reducible&& reducible, into_expresion<Iterator> const& expr)
	{
		return into(std::forward<Reducible>(reducible), expr.iterator);
	}

    template<typename Reducible, typename Iterator>
	Iterator operator|(Reducible&& reducible, into_expresion<Iterator>&& expr)
	{
		return into(std::forward<Reducible>(reducible), std::move(expr.iterator));
	}
}

/**
* Reduces a reducible into a sequence.
* This accumulates each element of the reducible to be piped in
* into the sequence written to by the iterator.
* @param iterator The output iterator to be used to write the accumulated sequence.
* @returns A implementation-specific helper object that can be combined with a reducible to accumulate the reducible.
*/
template<typename Iterator>
detail::into_expresion<typename std::decay<Iterator>::type>
into(Iterator&& iterator)
{
	return detail::into_expresion<typename std::decay<Iterator>::type>(std::forward<Iterator>(iterator));
}

WENDA_REDUCERS_NAMESPACE_END

#endif