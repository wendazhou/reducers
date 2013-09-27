#ifndef WENDA_REDUCERS_H_INCLUDED
#define WENDA_REDUCERS_H_INCLUDED

/**
* @file reducers.h
* This file contains includes all main subheaders of the wenda/reducers library.
* It is provided for convenience and ease of use. The user can also include only
* the required headers manually if he wishes to do so.
*/

/**
* @mainpage wenda/reducers
*
* @section intro_sec Introduction
* The wenda/reducers library implements a composable way of operating over sequences
* and collections with the notion of reducers, which are similar to their clojure [counterparts](http://clojure.org/reducers)
* from which they are inspired.
*
* @section gs_sec Getting started
* Getting started with the library is easy. Simply `#include <wenda/reducers.h>` and you're ready to go.
* The library works by default with all C++ ranges, which includes `std::vector`, arrays and any other range you can come with.
* It can also be extended to work with any collection that has a notion of being reducible by overloading a simple reduce() function.
* To showcase the functionality of the library, here is a complete self-contained example that computes the sum of the triple of all
* even integers from 1 to 5.
~~~
#include <iostream>
#include <functional>
#include <wenda/reducers.h>

namespace r = wenda::reducers;

int main()
{
    int data[]{1, 2, 3, 4, 5};

    std::cout <<
        data
        | r::filter([](int n) { return n % 2 == 0; })
        | r::map([](int n) { return n * 3 })
        | r::reduce(std::plus<int>(), 0);
}
~~~
* Note that this example is not optimal, as sequences of integers can be simply generated using reducible generators.
* Now, we note several interesting aspects:
* - first of all, we see that data, while being a plain array, works perfectly well with the library. Indeed, all ranges
*   are reducibles by default.
* - secondly, we see that the library overloads the bitwise-or `operator|`, and repurposes it to stand for "pipe" as in a shell.
*   This allows the produced code to be much more readable in its intention, and makes each element easily composable.
* - lastly, we see that it is extremely easy to take a reducible, in this case a plain array, and map() and filter() its elements
*   in a composable manner to introduce interesting behaviour.
*/

#include "reducers/reducers_common.h"
#include "reducers/reduce.h"

#include "reducers/transformers/collect.h"
#include "reducers/transformers/filter.h"
#include "reducers/transformers/map.h"

#include "reducers/into.h"

#include "reducers/reducibles/range_reducible.h"
#include "reducers/reducibles/iterator_pair_reducible.h"
#include "reducers/reducibles/sequence_reducible.h"

#include "reducers/monoid/monoid.h"
#include "reducers/monoid/monoid_reduce.h"

#endif // WENDA_REDUCERS_H_INCLUDED