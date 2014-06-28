/**
 * Talloc is a replacement for the standard memory allocation routines that
 * provides structure aware allocations.
 *
 * @author Dario Sneidermanis
 */

#ifndef __TALLOC_H__
#define __TALLOC_H__

#include <stddef.h>

/**
 * Allocate a (contiguous) memory chunk.
 *
 * @param size    amount of memory requested (in bytes).
 * @param parent  pointer to previously talloc'ed memory chunk from which this
 *                chunk depends, or NULL.
 *
 * @return pointer to the allocated memory chunk, or NULL if there was an error.
 */
void *talloc(size_t size, void *parent);

/**
 * Allocate a zeroed (contiguous) memory chunk.
 *
 * @param size    amount of memory requested (in bytes).
 * @param parent  pointer to previously talloc'ed memory chunk from which this
 *                chunk depends, or NULL.
 *
 * @return pointer to the allocated memory chunk, or NULL if there was an error.
 */
void *tzalloc(size_t size, void *parent);

/**
 * Modify the size of a talloc'ed memory chunk.
 *
 * @param mem   pointer to previously talloc'ed memory chunk.
 * @param size  amount of memory requested (in bytes).
 *
 * @return pointer to the allocated memory chunk, or NULL if there was an error.
 */
void *trealloc(void *mem, size_t size);

/**
 * Deallocate a talloc'ed memory chunk and all the chunks depending on it.
 *
 * @param mem  pointer to previously talloc'ed memory chunk.
 *
 * @return always NULL, can be safely ignored.
 */
void *tfree(void *mem);

/**
 * Get the parent of a talloc'ed memory chunk (the chunk on which it depends).
 *
 * @param mem  pointer to previously talloc'ed memory chunk.
 *
 * @return pointer to the parent memory chunk (could be NULL).
 */
void *talloc_get_parent(void *mem);

/**
 * Change the parent of a talloc'ed memory chunk. This will affect the
 * dependencies of the entire subtree rooted at the given chunk.
 *
 * @param mem     pointer to previously talloc'ed memory chunk.
 * @param parent  pointer to previously talloc'ed memory chunk from which this
 *                chunk depends, or NULL.
 */
void talloc_set_parent(void *mem, void *parent);

/**
 * Remove a talloc'ed memory chunk from the dependency tree, taking care of its
 * children (they will depend on parent).
 *
 * @param mem     pointer to previously talloc'ed memory chunk.
 * @param parent  pointer to previously talloc'ed memory chunk from which this
 *                chunk's children will depend, or NULL.
 */
void talloc_steal(void *mem, void *parent);

#endif /* __TALLOC_H__ */

