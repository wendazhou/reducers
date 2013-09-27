#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/monoid/monoid_reduce.h>
#include <wenda/reducers/reducibles/range_reducible.h>

#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace r = WENDA_REDUCERS_NAMESPACE;

namespace tests
{
	TEST_CLASS(MonoidTests)
	{
		TEST_METHOD(AdditiveMonoid_Has_Correct_Element_Type)
		{
			auto value = std::is_same<r::additive_monoid<int>::element_t, int>::value;

			Assert::IsTrue(value);
		}

		TEST_METHOD(AdditiveMonoid_Operation_Behaves_Like_Add)
		{
			auto op = r::additive_monoid<int>::operation_t();

			auto result = op(4, 5);

			Assert::AreEqual(4 + 5, 9);
		}

		TEST_METHOD(AdditiveMonoid_Unit_Returns_Correct_Value)
		{
			auto unit = r::additive_monoid<int>::unit();

			Assert::AreEqual(0, unit);
		}

		TEST_METHOD(Reduce_Over_Additive_Monoid_Gives_Correct_Result)
		{
			using r::reduce;
			int data[]{1, 2, 3, 4, 5};

			auto result = reduce<r::additive_monoid<int>>(data);

			Assert::AreEqual(1 + 2 + 3 + 4 + 5, result);
		}

		TEST_METHOD(Reduce_Over_Additive_Monoid_Gives_Correct_Result_Pipe)
		{
			using r::reduce;
			int data[]{1, 2, 3, 4, 5};

			auto result = data | reduce<r::additive_monoid<int>>();

			Assert::AreEqual(1 + 2 + 3 + 4 + 5, result);
		}
	};
}