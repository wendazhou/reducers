#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/reduce.h>
#include <wenda/reducers/range_reducible.h>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WENDA_REDUCERS_NAMESPACE;

namespace tests
{
	TEST_CLASS(Reduce_Tests)
	{
		TEST_METHOD(Can_Reduce_With_Pipe_Expression)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };
			auto result = make_range_reducible(data) | reduce(std::plus<int>(), 0);
			Assert::AreEqual(1 + 2 + 3 + 4 + 5 + 6, result);
		}
	};
}