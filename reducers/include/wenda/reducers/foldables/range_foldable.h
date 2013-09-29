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
#include "../reducibles/iterator_pair_reducible.h"
#include "../reduce.h"
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
    template<typename Reduce>
	struct range_reduce_function
	{
		Reduce const& reducer;

		range_reduce_function(Reduce const& reducer)
			: reducer(reducer)
		{}

        template<typename Beg, typename End, typename ReduceType>
		typename std::decay<ReduceType>::type operator()(Beg&& beg, End&& end, ReduceType&& seed) const
		{
			return make_iterator_pair_reducible(std::forward<Beg>(beg), std::forward<End>(end))
				| reduce(reducer, std::forward<ReduceType>(seed));
		}
	};

    template<typename Range, typename Reduce, typename Combine>
	typename std::decay<typename std::result_of<Combine()>::type>::type 
	fold_range_impl(Range&& range, Reduce&& reduce, Combine&& combine)
	{
		using std::begin;
		using std::end;
#ifdef _MSC_VER
		return concurrency::parallel_reduce(
			begin(range), end(range), 
			combine(),
			range_reduce_function<typename std::decay<Reduce>::type>(std::forward<Reduce>(reduce)),
            combine);
#else
		static_assert(false, "no implementation of fold_range_impl available for your platform.");
#endif
	}
}

/**
* Overload of fold() for C++ ranges.
* @param range A C++ range that is to be folded.
*/
template<typename Range, typename Reduce, typename Combine>
typename std::enable_if<
    detail::enable_range_fold<Range, Reduce, Combine>::value,
    typename std::decay<typename std::result_of<Combine()>::type>::type
>::type
fold(Range&& range, Reduce&& reduce, Combine&& combine)
{
	return detail::fold_range_impl(std::forward<Range>(range), std::forward<Reduce>(reduce), std::forward<Combine>(combine));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_FOLDABLE_RANGE_FOLDABLE_H_INCLUDED