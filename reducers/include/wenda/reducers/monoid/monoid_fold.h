#ifndef WENDA_REDUCERS_MONOID_MONOID_FOLD_H_INCLUDED
#define WENDA_REDUCERS_MONOID_MONOID_FOLD_H_INCLUDED

/**
* @file monoid_fold.h
* This file includes overloads of the fold() function
* templated for monoids.
*/

#include "../reducers_common.h"

#include <utility>

#include "monoid.h"
#include "../fold.h"

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
    template<typename Monoid>
	struct monoid_combine
		: monoid_traits<Monoid>::operation_t
	{
		typedef typename monoid_traits<Monoid>::element_t element_t;

		element_t operator()() const
		{
			return monoid_traits<Monoid>::unit();
		}

		using monoid_traits<Monoid>::operation_t::operator();
	};
}

/**
* Folds the given @p foldable over the structure of the given @p Monoid.
* Note that it is usually easier to use the no argument version fold(void)
* in a pipe expression.
* @param foldable The foldable to fold.
* @tparam Monoid The monoid structure over which to fold the @p foldable.
* @returns The result of folding the sequence over the given monoid structure.
* @sa fold(void)
*/
template<typename Monoid, typename Foldable>
typename monoid_traits<Monoid>::element_t
fold(Foldable&& foldable)
{
	typedef detail::monoid_combine<Monoid> combine_t;
	typedef typename monoid_traits<Monoid>::operation_t reduce_t;
	return fold(std::forward<Foldable>(foldable), reduce_t(), combine_t());
}

namespace detail
{
    template<typename Monoid>
	struct monoid_fold_expression
	{
        // left empty, all information is in type
	};

    template<typename Monoid, typename Foldable>
    typename monoid_traits<Monoid>::element_t
	operator|(Foldable&& foldable, monoid_fold_expression<Monoid>)
	{
		return fold<Monoid>(std::forward<Foldable>(foldable));
	}
}

/**
* Creates an object, that when combined with a foldable through the
* bitwise-or operator, folds the given foldable using the structure of the
* given monoid.
* @tparam Monoid A type describing the structure of the monoid that is folded over.
* @returns An unspecified object, that when combined with a foldable, folds the sequence.
*/
template<typename Monoid>
detail::monoid_fold_expression<Monoid> fold()
{
	return detail::monoid_fold_expression<Monoid>();
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_MONOID_MONOID_FOLD_H_INCLUDED