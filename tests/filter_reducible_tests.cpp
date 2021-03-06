#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/transformers/filter.h>
#include <wenda/reducers/reducibles/range_reducible.h>
#include <wenda/reducers/reduce.h>
#include <wenda/reducers/foldables/range_foldable.h>
#include <wenda/reducers/monoid/monoid_fold.h>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WENDA_REDUCERS_NAMESPACE;

namespace tests
{
	TEST_CLASS(FilterReducibleTests)
	{
		TEST_METHOD(filter_produces_correct_result)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };

			auto result = filter(make_range_reducible(data), [](int n){ return n % 2 == 0; })
				          | reduce(std::plus<int>(), 0);

			Assert::AreEqual(2 + 4 + 6, result);
		}

		TEST_METHOD(Filter_Can_Be_Used_In_Pipe_Expression_RValue)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };

			auto result =
				make_range_reducible(data)
				| filter([](int n){return n % 2 == 0; })
				| reduce(std::plus<int>(), 0);

			Assert::AreEqual(2 + 4 + 6, result);
		}

		TEST_METHOD(Filter_Can_Be_Used_In_Pipe_Expression_LValue)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };

			auto reducible =
				make_range_reducible(data)
				| filter([](int n){return n % 2 == 0; });
            auto result = 
				reducible
				| reduce(std::plus<int>(), 0);

			Assert::AreEqual(2 + 4 + 6, result);
		}

		TEST_METHOD(Filter_Can_Be_Folded)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5 };

			auto result =
				data
				| filter([](int n) { return n % 2 == 0; })
				| fold<additive_monoid<int>>();

			Assert::AreEqual(2 + 4, result);
		}
	};
}