#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/filter.h>
#include <wenda/reducers/range_reducible.h>

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
				.reduce(std::plus<int>(), 0);

			Assert::AreEqual(2 + 4 + 6, result);
		}
	};
}