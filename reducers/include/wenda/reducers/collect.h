#ifndef WENDA_REDUCERS_COLLECT_H_INCLUDED
#define WENDA_REDUCERS_COLLECT_H_INCLUDED

#include "reducers_common.h"

#include <utility>
#include <type_traits>

WENDA_REDUCERS_NAMESPACE_BEGIN

namespace detail
{
    template<typename Reducer, typename Expander>
	struct collect_reducing_function
	{
		Reducer const& reducer;
		Expander const& expander;

		collect_reducing_function(Reducer const& reducer, Expander const& expander)
			: reducer(reducer), expander(expander)
		{
		}

        template<typename Value, typename Seed>
		typename std::decay<Seed>::type operator()(Value&& value, Seed&& seed)
		{
			return expander(std::forward<Value>(value)).reduce(reducer, std::forward<Seed>(seed));
		}
	};

    template<typename ExpandFunction>
	struct collect_reducible_expression
	{
		ExpandFunction expandFunction;

		collect_reducible_expression(ExpandFunction expandFunction)
			: expandFunction(std::move(expandFunction))
		{
		}
	};
}

template<typename Reducible, typename ExpandFunction>
class collect_reducible
{
	Reducible reducible;
	ExpandFunction expandFunction;
public:
	collect_reducible(Reducible reducible, ExpandFunction expandFunction)
		: reducible(std::move(reducible)), expandFunction(std::move(expandFunction))
	{
	}

    template<typename Reducer, typename Seed>
	typename std::decay<Seed>::type reduce(Reducer&& reducer, Seed&& seed) const
	{
		typedef detail::collect_reducing_function<typename std::decay<Reducer>::type, ExpandFunction> collect_reducer_t;
		return reducible.reduce(collect_reducer_t(std::forward<Reducer>(reducer), expandFunction), std::forward<Seed>(seed));
	}
};

namespace detail
{
    template<typename Reducible, typename ExpandFunction>
    collect_reducible<typename std::decay<Reducible>::type, ExpandFunction>
	operator|(Reducible&& reducible, collect_reducible_expression<ExpandFunction> const& expr)
	{
		typedef collect_reducible<typename std::decay<Reducible>::type, ExpandFunction> return_t;
		return return_t(std::forward<Reducible>(reducible), expr.expandFunction);
	}

    template<typename Reducible, typename ExpandFunction>
    collect_reducible<typename std::decay<Reducible>::type, ExpandFunction>
	operator|(Reducible&& reducible, collect_reducible_expression<ExpandFunction>&& expr)
	{
		typedef collect_reducible<typename std::decay<Reducible>::type, ExpandFunction> return_t;
		return return_t(std::forward<Reducible>(reducible), std::move(expr.expandFunction));
	}
}

template<typename Reducible, typename ExpandFunction>
collect_reducible<typename std::decay<Reducible>::type, typename std::decay<ExpandFunction>::type>
collect(Reducible&& reducible, ExpandFunction&& expandFunction)
{
	typedef collect_reducible<typename std::decay<Reducible>::type, typename std::decay<ExpandFunction>::type> return_type;
	return return_type(std::forward<Reducible>(reducible), std::forward<ExpandFunction>(expandFunction));
}

template<typename ExpandFunction>
detail::collect_reducible_expression<typename std::decay<ExpandFunction>::type>
collect(ExpandFunction&& expandFunction)
{
	typedef detail::collect_reducible_expression<typename std::decay<ExpandFunction>::type> return_t;
	return return_t(std::forward<ExpandFunction>(expandFunction));
}

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_COLLECT_H_INCLUDED