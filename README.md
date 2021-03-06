# wenda/reducers

This is a C++ implementation of a reducer-like library as exists in Clojure (see http://clojure.org/reducers)

It implements a basic reducible concept for C++ ranges (classes that can be iterated using a range for loop) as well as iterator pairs. It also implements basic transformations for these reducibles, including filter, map and collect.

# Reducible

A _reducible_ is an object that knows how to reduce itself, that is, perform some sort of accumulation over its elements. For example, all STL containers can be reduced using `std::accumulate`, and such collections represent indeed the typical case of reducibles. 

Additionally, all reducibles can be transformed by various standard transformers, such as `filter` or `map`. This is explained in detail in the Clojure blog [post](http://clojure.com/blog/2012/05/08/reducers-a-library-and-model-for-collection-processing.html).

# Getting started

As we have already seen, all ranges are reducible. Hence, to get started with the library, the simplest example would be
```c++
#include <iostream>
#include <functional>
#include <wenda/reducers.h>

namespace r = wenda::reducers;

int main()
{
    using r::reduce;
    int data[]{1, 2, 3, 4, 5};
	std::cout << reduce(data, std::plus<int>(), 0) << std::endl;
	return 0;
}
```
which prints out the sum of the numbers from 1 to 5.

A few notes on what is happening:
- first of all, a _reducible_ is an object for which there exists a `reduce` function. In this case, the library provides an implementation of `reduce` for arrays, but any object can be made reducible by providing an overload of the reduce function in its namespace (to be found by ADL).
- secondly, in this case, the reduce statement is exactly equivalent to `std::accumulate(std::begin(data), std::end(data))`, and in general, `reduce` is just a generalisation of accumulate.

Now, to make things more interesting, let us compute instead the sum of all the even numbers from 1 to 5. We assume that we have the same includes as previously, and the same namespace alias.
```c++
int sum_even_1_to_5()
{
    int data[]{1, 2, 3, 4, 5};
	return reduce(filter(data, [](int n){ return n % 2 == 0; }), std::plus<int>(), 0);
}
```
This showcases our first _transformer_, which is `filter`. If you are familiar with LinQ in .Net, or boost::range, this should seem familiar. The advantages of these transformers is that they are composable. Let's see for example, the sum of the triple of all the even numbers from 1 to 5.
```c++
int sum_triple_even_1_to_5()
{
   int data[]{1, 2, 3, 4, 5};
   return reduce(map(filter(data, [](int n){ return n % 2 == 0; }), [](int n){ return n*3; }), std::plus<int>(), 0);
}
```
As we see, we have used the _transformer_ `map` to project all the elements to a new value.

Although this is composable, it is quite akward to write, as all the operations start piling in front of the expression. In order to solve this problem, the library also uses a secondary (and preferred) syntax, overloading the `|` operator (bitwise-or) to mean pipe (in bash style). You can think of this operator as forwarding the left hand side to become the first argument of the expression of the left hand side. Let's see that in action.
```c++
int sum_triple_even_1_to_5_improved()
{
   int data[]{1, 2, 3, 4, 5};
   return data
          | filter([](int n){ return n % 2 == 0; })
		  | map([](int n){ return n*3; })
		  | reduce(std::plus<int>(), 0);
}
```
