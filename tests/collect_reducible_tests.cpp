#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/transformers/collect.h>
#include <wenda/reducers/range_reducible.h>
#include <wenda/reducers/reduce.h>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WENDA_REDUCERS_NAMESPACE;

namespace tests
{
	TEST_CLASS(CollectReducibleTests)
	{
		TEST_METHOD(CollectReducible_Returns_Correct_Results)
		{
			std::vector<std::vector<int>> data{ { 1, 2, 3 }, { 4, 5, 6 } };

			auto result = collect(make_range_reducible(data), [](std::vector<int> const& d) { return make_range_reducible(d); })
				          | reduce(std::plus<int>(), 0);

			Assert::AreEqual(1 + 2 + 3 + 4 + 5 + 6, result);
		}

		TEST_METHOD(Collect_Can_Be_Used_In_Pipe_Expression_RValue)
		{
			std::vector<std::vector<int>> data{ { 1, 2, 3 }, { 4, 5, 6 } };

			auto result = 
				make_range_reducible(data)
				| collect([](std::vector<int> const& d) { return make_range_reducible(d); })
				| reduce(std::plus<int>(), 0);

			Assert::AreEqual(1 + 2 + 3 + 4 + 5 + 6, result);
		}

		TEST_METHOD(Collect_Can_Be_Used_In_Pipe_Expression_LValue)
		{
			std::vector<std::vector<int>> data{ { 1, 2, 3 }, { 4, 5, 6 } };

			auto reducible =
				make_range_reducible(data)
				| collect([](std::vector<int> const& d) { return make_range_reducible(d); });
            auto result = 
                reducible
				| reduce(std::plus<int>(), 0);

			Assert::AreEqual(1 + 2 + 3 + 4 + 5 + 6, result);
		}
	};
}