#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/into.h>
#include <wenda/reducers/reduce.h>
#include <wenda/reducers/reducibles/range_reducible.h>

#include <vector>
#include <set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WENDA_REDUCERS_NAMESPACE;

namespace tests
{
	TEST_CLASS(IntoTests)
	{
		TEST_METHOD(Into_Can_Accumulate_To_Vector)
		{
			std::vector<int> data{ 1, 2, 3, 4 };

			std::vector<int> result;

			data | into(std::back_inserter(result));

			Assert::IsTrue(std::equal(data.begin(), data.end(), result.begin()));
		}

		TEST_METHOD(Into_Can_Accumulate_To_Set)
		{
			std::vector<int> data{ 1, 2, 1, 2 };

			std::set<int> result;

			data | into(std::inserter(result, result.begin()));

			Assert::IsTrue(result.size() == 2);
			Assert::IsTrue(result.find(1) != result.end());
			Assert::IsTrue(result.find(2) != result.end());
		}
	};
}