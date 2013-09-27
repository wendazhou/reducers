#include "stdafx.h"
#include "CppUnitTest.h"

#include <wenda/reducers/reducibles/range_reducible.h>
#include <wenda/reducers/reduce.h>

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

		TEST_METHOD(RangeReducible_Reduce_Empty_Range_Returns_Seed)
		{
			std::vector<int> data;

			auto reducible = make_range_reducible(data);

			auto result = reducible.reduce(std::plus<int>(), 5);

			Assert::AreEqual(5, result);
		}

		TEST_METHOD(RangeReducible_Can_Make_From_Array)
		{
			int data []{1, 2, 3, 4, 5, 6};

			auto reducible = make_range_reducible(data);

			auto result = reducible.reduce(std::plus<int>(), 0);

			Assert::AreEqual(1 + 2 + 3 + 4 + 5 + 6, result);
		}

		TEST_METHOD(RangeReducible_Has_Reducible_Member_Function)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };

			auto reducible = make_range_reducible(data);

			auto value = detail::has_reducible_member_function<decltype(reducible), std::plus<int>, int>::value;

			Assert::IsTrue(value);
		}

		TEST_METHOD(Is_Iterator_Correctly_Identifies_Iterators)
		{
			auto value = detail::is_iterator<std::vector<int>::iterator>::value;
			Assert::IsTrue(value);
		}

		TEST_METHOD(Is_Range_Correctly_Identifies_Ranges)
		{
			auto value = detail::is_range<std::vector<int> >::value;
			Assert::IsTrue(value);
		}

		TEST_METHOD(Can_Directly_Reduce_Vector_Range)
		{
			std::vector<int> data{ 1, 2, 3, 4, 5, 6 };

			auto result = data | reduce(std::plus<int>(), 0);

			Assert::AreEqual(1 + 2 + 3 + 4 + 5 + 6, result);
		}

		TEST_METHOD(Can_Directly_Reduce_Array_Range)
		{
			int data[]{ 1, 2, 3, 4, 5, 6 };

			auto result = data | reduce(std::plus<int>(), 0);

			Assert::AreEqual(1 + 2 + 3 + 4 + 5 + 6, result);
		}
	};
}