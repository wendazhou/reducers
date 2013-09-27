#ifndef WENDA_REDUCERS_MONOID_MONOID_H_INCLUDED
#define WENDA_REDUCERS_MONOID_MONOID_H_INCLUDED

/**
* @file monoid.h
* This file contains the basic definition for the @ref monoid_traits class
* that is used throughout library.
*/

#include "../reducers_common.h"

#include <functional>

WENDA_REDUCERS_NAMESPACE_BEGIN

/**
* A monoid is a type that can be added to itself, with the addition
* admitting an identity (i.e. a zero) and being associative (i.e. the parentheses don't matter).
* Specialize this structure to define the monoid operations on your type.
*/
template<typename Monoid>
struct monoid_traits
{
	/**
    * This type indicates the type of the elements in the monoid.
	*/
	typedef typename Monoid::element_t element_t;
	/**
    * This type indicates the type of the operation (add) in the monoid.
    * It must be a functor type, invokable with signature (T, T) -> T.
	*/
	typedef typename Monoid::operation_t operation_t;
	/**
    * This function returns the unit value for a given monoid,
    * that is, an element x that verifies, for all y, x + y = y.
    * @returns An element of type element_t that represents an identity value.
	*/
	static element_t unit() { return Monoid::unit(); }
};

/**
* This type represents an additive monoid over a given type,
* that is, if the type has an operation + satisfying the monoid laws,
* and if it has a default constructor initializing the type to a unit value,
* then its additive monoid is represented by this type.
*/
template<typename T>
struct additive_monoid
{
	typedef T element_t;
	typedef std::plus<T> operation_t;
	static element_t unit() { return T(); }
};

WENDA_REDUCERS_NAMESPACE_END

#endif // WENDA_REDUCERS_MONOID_MONOID_H_INCLUDED