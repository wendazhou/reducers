#ifndef WENDA_REDUCERS_RANGE_REDUCIBLE_H_INCLUDED
#define WENDA_REDUCERS_RANGE_REDUCIBLE_H_INCLUDED

#include "reducers_common.h"

#include <iterator>
#include <type_traits>

#include "reduce.h"

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

    template<typename T, typename FuncType, typename Seed>
	class enable_range_reducible
	{
		template<typename U> static std::false_type test(
			typename std::enable_if<has_reducible_member_function<U, FuncType, Seed>::value, int
		>::type);

		template<typename U> static std::true_type test(
			typename std::enable_if<is_range<U>::value, long>::type);

		template<typename U> static std::false_type test(...);

	public:
		typedef decltype(test<T>(0)) type;
		static const bool value = type::value;
	};
}

template<typename Range>
class range_reducible
{
	Range const& range;

public:
	range_reducible(Range const& range)
		: range(range)
	{}

    template<typename FunctionType, typename SeedType>
	SeedType reduce(FunctionType&& function, SeedType seed) const
	{
		for (auto&& val : range)
		{
			seed = function(val, std::move(seed));
		}

		return seed;
	}
};

template<typename Range>
range_reducible<Range> make_range_reducible(Range&& range)
{
	return range_reducible<Range>(std::forward<Range>(range));
}

template<typename Range, typename FuncType, typename Seed>
typename std::enable_if<detail::enable_range_reducible<Range, FuncType, Seed>::value, Seed>::type
reduce(Range&& range, FuncType&& function, Seed&& seed)
{
	return make_range_reducible(std::forward<Range>(range)).reduce(std::forward<FuncType>(function), std::forward<Seed>(seed));
}

WENDA_REDUCERS_NAMESPACE_END

#endif WENDA_REDUCERS_RANGE_REDUCIBLE_H_INCLUDED