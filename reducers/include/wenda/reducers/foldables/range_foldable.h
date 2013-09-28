#ifndef WENDA_REDUCERS_FOLDABLE_RANGE_FOLDABLE_H_INCLUDED
#define WENDA_REDUCERS_FOLDABLE_RANGE_FOLDABLE_H_INCLUDED

/**
* @file range_foldable.h
* This file contains a basic implementation of the fold() function for
* C++ ranges. The implementation is based on Microsoft's parallel patterns library for VC.
*/

#include "../reducers_common.h"

#include <utility>
#include <type_traits>
#include <iterator>

#ifdef _MSC_VER
#include <ppl.h>
#endif

#include "../detail/is_range.h"
#include "../fold.h"

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
	/**
    * This struct is a simple trait to determine whether the generic overload for ranges
    * should be enabled.
    * It is enabled in the case that the type is a range, and that it has no reducible() member function.
	*/
    template<typename Range, typename Function, typename Element>
	struct enable_range_fold
	{
		static const bool value =
			!has_foldable_member_function<Range, Function, Element>::value &&
			is_range<Range>::value;
	};
}

namespace detail
{
    template<typename Range, typename Function, typename Element>
	typename std::decay<Element>::type 
	fold_range_impl(Range&& range, Function&& function, Element&& identity)
	{
		using std::begin;
		using std::end;
#ifdef _MSC_VER
		return concurrency::parallel_reduce(begin(range), end(range), std::forward<Element>(identity), std::forward<Function>(function));
#else
		static_assert(false, "no implementation of fold_range_impl available for your platform.");
#endif
	}
}

/**
* Folds the given @p range using the given @p function and @p identity element.
* Note that it must be the case that @p identity is a left identity for the given @p function,
* and that the function is associative.
* @param range An object that models a C++ range to be folded.
* @param function The function to be used when folding the range.
* @param identity The identity value for the given @p function.
* @returns The result of aggregating all the values in the @p range using the given @p function.
*/
template<typename Range, typename Function, typename Element>
typename std::enable_if<
    detail::enable_range_fold<Range, Function, Element>::value,
    typename std::decay<Element>::type
>::type
fold(Range&& range, Function&& function, Element&& identity)
{
	return detail::fold_range_impl(std::forward<Range>(range), std::forward<Function>(function), std::forward<Element>(identity));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_FOLDABLE_RANGE_FOLDABLE_H_INCLUDED