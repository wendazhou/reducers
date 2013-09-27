#ifndef WENDA_REDUCERS_MONOID_REDUCE_H_INCLUDED
#define WENDA_REDUCERS_MONOID_REDUCE_H_INCLUDED

#include "../reducers_common.h"

#include <utility>

#include "monoid.h"
#include "../reduce.h"

WENDA_REDUCERS_NAMESPACE_BEGIN

/**
* Reduces the given reducible over a given monoid.
* This calls reduce with the unit value as seed and the monoid operation as reducing operation.
* @param reducible The reducible to be reduced.
* @tparam Monoid The monoid type over which the reduction should be performed.
*/
template<typename Monoid, typename Reducible>
typename monoid_traits<Monoid>::element_t reduce(Reducible&& reducible)
{
	return reduce(std::forward<Reducible>(reducible), monoid_traits<Monoid>::operation_t(), monoid_traits<Monoid>::unit());
}

namespace detail
{
	/**
    * This is an implementation class that allows
    * the use of monoid reductions in piped expressions.
	*/
    template<typename Monoid>
	struct monoid_reduce_expression
	{
        // class empty, all information is in type
	};

    template<typename Monoid, typename Reducible>
    typename monoid_traits<Monoid>::element_t
	operator|(Reducible&& reducible, monoid_reduce_expression<Monoid>)
	{
        // note that we take the second parameter by value as it's empty anyways.
		return reduce<Monoid>(std::forward<Reducible>(reducible));
	}
}

/**
* Reduces the given reducible over the indicated monoid.
* This is identical to the two argument expression,
* except that it is used in piped expressions.
* @returns An implementation object that combines using the | operator to reduce its left hand side.
*/
template<typename Monoid>
detail::monoid_reduce_expression<Monoid> reduce(void)
{
	return detail::monoid_reduce_expression<Monoid>();
}


WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_MONOID_REDUCE_H_INCLUDED