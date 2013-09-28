#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/fold.h>
#include <wenda/reducers/foldables/range_foldable.h>
#include <wenda/reducers/monoid/monoid_fold.h>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace r = WENDA_REDUCERS_NAMESPACE;

namespace tests
{
	TEST_CLASS(RangeFoldableTests)
	{
		TEST_METHOD(CanFoldArray)
		{
			int data []{1, 2, 3, 4, 5};

			auto result = data | r::fold<r::additive_monoid<int>>();

			Assert::AreEqual(1 + 2 + 3 + 4 + 5, result);
		}

		TEST_METHOD(CanFoldVector)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5 };

			auto result = data | r::fold<r::additive_monoid<int>>();

			Assert::AreEqual(1 + 2 + 3 + 4 + 5, result);
		}
	};
}