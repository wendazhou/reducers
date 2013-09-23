#include "stdafx.h"
#include "CppUnitTest.h"

#include <wenda/reducers/range_reducible.h>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WENDA_REDUCERS_NAMESPACE;

namespace tests
{		
	TEST_CLASS(RangeReducibleTests)
	{
	public:
		TEST_METHOD(RangeReducible_Can_Instantiate_From_Vector)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };

			auto reducible = make_range_reducible(data);

			auto result = reducible.reduce(std::plus<int>(), 0);

			Assert::AreEqual(1 + 2 + 3 + 4 + 5 + 6, result);
		}
	};
}