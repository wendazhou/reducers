#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/transformers/map.h>
#include <wenda/reducers/reducibles/range_reducible.h>
#include <wenda/reducers/reduce.h>
#include <wenda/reducers/foldables/range_foldable.h>
#include <wenda/reducers/monoid/monoid_fold.h>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WENDA_REDUCERS_NAMESPACE;

namespace tests
{
	TEST_CLASS(MapReducibleTests)
	{
		TEST_METHOD(MapReducible_Returns_Correct_Result)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };

			auto result = map(make_range_reducible(data), [](int n){ return n * 2; })
		                  | reduce(std::plus<int>(), 0);

			Assert::AreEqual((1 + 2 + 3 + 4 + 5 + 6) * 2, result);
		}

		TEST_METHOD(Map_Can_Use_In_Pipe_Expression_RValue)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };
			auto result =
				make_range_reducible(data)
				| map([](int n){return n * 2; })
				| reduce(std::plus<int>(), 0);

			Assert::AreEqual((1 + 2 + 3 + 4 + 5 + 6) * 2, result);
		}

		TEST_METHOD(Map_Can_Use_In_Pipe_Expression_LValue)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };
			auto reducible =
				make_range_reducible(data)
				| map([](int n){return n * 2; });

            auto result = reducible | reduce(std::plus<int>(), 0);

			Assert::AreEqual((1 + 2 + 3 + 4 + 5 + 6) * 2, result);
		}

		TEST_METHOD(Map_Can_Fold_In_Pipe_Expression_RValue)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };
			auto result =
				data
				| map([](int n){return n * 2; })
				| fold<additive_monoid<int>>();

			Assert::AreEqual((1 + 2 + 3 + 4 + 5 + 6) * 2, result);
		}

		TEST_METHOD(Map_Can_Fold_In_Pipe_Expression_LValue)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };
			auto foldable =
				data
				| map([](int n){return n * 2; });
            auto result = foldable | fold<additive_monoid<int>>();

			Assert::AreEqual((1 + 2 + 3 + 4 + 5 + 6) * 2, result);
		}
	};
}