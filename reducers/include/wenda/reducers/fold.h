#ifndef WENDA_REDUCERS_FOLD_H_INCLUDED
#define WENDA_REDUCERS_FOLD_H_INCLUDED

/**
* @file fold.h
* This file contains a generic implementation of the fold() function.
* fold() is similar to reduce(), but must operate over a monoid, which
* enables opportunities to parallelise the operation.
*/

#include "reducers_common.h"

#include <utility>
#include <type_traits>

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
    template<typename T, typename FoldFunction, typename ElementType>
	class has_foldable_member_function
	{
		template<typename U> static std::true_type test(
			typename std::add_pointer<decltype(std::declval<U>().fold(std::declval<FoldFunction>(), std::declval<Elementtype>()))>::type);
		template<typename U> static std::false_type test(...);
	public:
		typedef decltype(test<T>(nullptr)) type;
		static const bool value = type::value;
	};
}

template<typename Foldable, typename FoldFunction, typename ElementType>
typename std::enable_if<
	detail::has_foldable_member_function<Foldable, FoldFunction, ElementType>::value,
	typename std::decay<ElementType>::type
>::type
fold(Foldable&& foldable, FoldFunction&& foldFunction, ElementType&& identity)
{
	return foldable.fold(std::forward<FoldFunction>(foldFunction), std::forward<ElementType>(identity));
}

namespace detail
{
	template<typename FoldFunction, typename ElementType>
	struct fold_expression
	{
		FoldFunction function;
		ElementType element;

		fold_expression(FoldFunction function, ElementType element)
			: function(std::move(function)), element(std::move(element))
		{
		}
	};

	template<typename Foldable, typename FoldFunction, typename ElementType>
	typename std::decay<ElementType>::type
	operator|(Foldable&& foldable, fold_expression<FoldFunction, ElementType> const& expr)
	{
		return fold(std::forward<Foldable>(foldable), expr.function, expr.element);
	}

    template<typename Foldable, typename FoldFunction, typename ElementType>
    typename std::decay<ElementType>::type
	operator|(Foldable&& foldable, fold_expression<FoldFunction, ElementType>&& expr)
	{
		return fold(std::forward<Foldable>(foldable), std::move(expr.function), std::move(expr.element));
	}
}

template<typename FoldFunction, typename ElementType>
detail::fold_expression<typename std::decay<FoldFunction>::type, typename std::decay<ElementType>::type>
fold(FoldFunction&& function, ElementType&& identity)
{
	typedef detail::fold_expression<typename std::decay<FoldFunction>::type, typename std::decay<ElementType>::type> return_t;
	return return_t(function, identity);
}

WENDA_REDUCERS_NAMESPACE_END

#endif