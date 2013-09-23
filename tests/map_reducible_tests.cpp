#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/map.h>
#include <wenda/reducers/range_reducible.h>

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
				.reduce(std::plus<int>(), 0);

			Assert::AreEqual((1 + 2 + 3 + 4 + 5 + 6) * 2, result);
		}
	};
}