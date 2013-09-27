#ifndef WENDA_REDUCERS_REDUCERS_COMMON_H_INCLUDED
#define WENDA_REDUCERS_REDUCERS_COMMON_H_INCLUDED

/**
* @file reducers_common.h
* This file contains common macros used in this project.
*/

/**
* Main namespace of the wenda/reducers project
*/
#define WENDA_REDUCERS_NAMESPACE wenda::reducers

/**
* Opens the main namespace for the wenda/reducers library.
* In combination with #WENDA_REDUCERS_NAMESPACE_END, this allows
* the library to declare the namespace in a flexible way.
* @sa WENDA_REDUCERS_NAMESPACE_END
*/
#define WENDA_REDUCERS_NAMESPACE_BEGIN namespace wenda { namespace reducers {
/**
* Closes the main namespace for the wenda/reducers library.
* In combination with #WENDA_REDUCERS_NAMESPACE_BEGIN, this allows
* the library to declare the namespace in a flexible way.
* @sa WENDA_REDUCERS_NAMESPACE_BEGIN
*/
#define WENDA_REDUCERS_NAMESPACE_END } }

#endif // WENDA_REDUCERS_REDUCERS_COMMON_H_INCLUDED