#ifndef WENDA_REDUCERS_DETAIL_IS_RANGE_H_INCLUDED
#define WENDA_REDUCERS_DETAIL_IS_RANGE_H_INCLUDED

/**
* @file is_range.h
* This file contains implementation helpers for implementing range reducibles and foldables.
* It contains a trait type @ref is_range to determine whether a type models a range concept.
*/

#include "../reducers_common.h"

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
    template<typename T>
	class is_iterator
	{
		template<typename U> static std::true_type test(typename std::enable_if<!std::is_same<typename std::iterator_traits<U>::value_type, void>::value, int>::type);
		template<typename U> static std::false_type test(...);
	public:
		typedef decltype(test<T>(0)) type;
		static const bool value = type::value;
	};

    template<typename T>
	class is_range
	{
		template<typename U> static std::true_type test(
			typename std::enable_if< 
				is_iterator<decltype(begin(std::declval<U>()))>::value && is_iterator<decltype(end(std::declval<U>()))>::value,
			    int
			>::type);
		template<typename U> static std::true_type test(
            typename std::enable_if<
			    std::extent<typename std::remove_reference<U>::type>::value != 0,
                long
			>::type);

		template<typename U> static std::false_type test(...);
	public:
		typedef decltype(test<T>(0)) type;
		static const bool value = type::value;
	};
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_DETAIL_IS_RANGE_H_INCLUDED