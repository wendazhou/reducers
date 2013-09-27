#include <wenda/reducers/iterator_pair_reducible.h>
#include <wenda/reducers/reduce.h>
#include <wenda/reducers/transformers/map.h>
#include <wenda/reducers/transformers/filter.h>

using namespace WENDA_REDUCERS_NAMESPACE;

int sum_of_even(int* data, std::size_t length)
{
	return
		make_iterator_pair_reducible(data, data + length)
		| filter([](int n){ return n % 2 == 0; })
		| reduce(std::plus<int>(), 0);
}

int sum_of_even_for_loop(int* data, std::size_t length)
{
	int acc = 0;

	for (std::size_t i = 0; i < length; i++)
	{
		int n = data[i];


		if (n % 2 == 0)
		{
			acc += n;
		}
	}

	return acc;
}