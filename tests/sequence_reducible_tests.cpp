#include "stdafx.h"
#include <CppUnitTest.h>

#include <wenda/reducers/reducibles/sequence_reducible.h>
#include <wenda/reducers/reduce.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WENDA_REDUCERS_NAMESPACE;

namespace tests
{
	TEST_CLASS(SequenceReducibleTests)
	{
		TEST_METHOD(sequence_reducible_is_correct_for_int)
		{
			auto result = 
				make_sequence_reducible(1, 7, 1)
				| reduce(std::plus<int>(), 0);

			Assert::AreEqual(1 + 2 + 3 + 4 + 5 + 6, result);
		}

		TEST_METHOD(sequence_reducible_is_correct_for_step_2)
		{
			auto result =
				make_sequence_reducible(1, 10, 2)
				| reduce(std::plus<int>(), 0);

			Assert::AreEqual(1 + 3 + 5 + 7 + 9, result);
		}
	};
}