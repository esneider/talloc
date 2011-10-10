/**
 * @file talloc.h
 *
 * @brief Provides structure aware allocations
 *
 * @author Dario Sneidermanis
 */

#ifndef _TALLOC_H_
#define _TALLOC_H_

#include <stddef.h>


/**
 * Allocate memory
 *
 * @param size    amount of memory requested
 * @param parent  pointer to previously talloc'ed memory from wich this chunk
 *                depends or NULL
 *
 * @return a pointer to the allocated memory
 */
void* talloc ( size_t size, void* parent );


/**
 * Allocate zeroed memory
 *
 * @param size    amount of memory requested
 * @param parent  pointer to previously talloc'ed memory from wich this chunk
 *                depends or NULL
 *
 * @return a pointer to the allocated memory
 */
void* tcalloc ( size_t size, void* parent );


/**
 * Re-allocate memory
 *
 * @param mem   pointer to previously talloc'ed memory
 * @param size  amount of memory requested
 *
 * @return a pointer to the allocated memory if successful, NULL otherwise
 */
void* trealloc ( void* mem, size_t size );


/**
 * Free memory
 *
 * @param mem  pointer to previously talloc'ed memory
 */
void tfree ( void* mem );


/**
 * Get parent of talloc'ed memory
 *
 * @param mem  pointer to previously talloc'ed memory
 *
 * @return pointer to previously talloc'ed memory from which this chunk depends
 */
void* talloc_get_parent ( void* mem );


/**
 * Set parent of talloc'ed memory
 *
 * @param mem     pointer to previously talloc'ed memory
 * @param parent  pointer to previously talloc'ed memory from wich this chunk
 *                depends or NULL
 */
void talloc_set_parent ( void* mem, void* parent );


/**
 * Remove chunk of talloc'ed memory from dependency chain.
 *
 * @param mem     pointer to previously talloc'ed memory
 * @param parent  pointer to previously talloc'ed memory from wich this chunk's
 *                sons will depend or NULL
 */
void talloc_steal ( void* mem, void* parent );


#endif /* _TALLOC_H_ */

