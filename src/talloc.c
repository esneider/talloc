/**
 * Talloc is a replacement for the standard memory allocation routines that
 * provides structure aware allocations.
 *
 * @author Dario Sneidermanis
 *
 * Each chunk of talloc'ed memory has a header of the following form:
 *
 * +---------+---------+---------+--------···
 * |  first  |  next   |  prev   | memory
 * |  child  | sibling | sibling | chunk
 * +---------+---------+---------+--------···
 *
 * Thus, a talloc hierarchy tree would look like this:
 *
 *   NULL <-- chunk --> NULL
 *              ^
 *              |
 *              +-> chunk <--> chunk <--> chunk --> NULL
 *                    |          |          ^
 *                    v          v          |
 *                   NULL       NULL        +-> chunk <--> chunk --> NULL
 *                                                |          |
 *                                                v          v
 *                                               NULL       NULL
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "talloc.h"

/**
 * Talloc tree node helpers.
 */

#define HEADER_SIZE (sizeof(void*) * 3)

#define  raw2usr(mem) (void*)((void**)(mem) + 3)
#define  usr2raw(mem) (void*)((void**)(mem) - 3)
#define    child(mem) (((void**)(mem))[-3])
#define     next(mem) (((void**)(mem))[-2])
#define     prev(mem) (((void**)(mem))[-1])
#define   parent(mem) prev(mem) /* Valid only when is_first(mem). */
#define  is_root(mem) (!prev(mem))
#define is_first(mem) (next(prev(mem)) != (mem))

/**
 * Initialize a raw chunk of memory.
 *
 * @param mem     pointer to a raw memory chunk.
 * @param parent  pointer to previously talloc'ed memory chunk from which this
 *                chunk depends, or NULL.
 *
 * @return pointer to the allocated memory chunk, or NULL if there was an error.
 */
static void *talloc_init(void *mem, void *parent) {

    if (!mem)
        return NULL;

    memset(mem, 0, HEADER_SIZE);
    mem = raw2usr(mem);

    talloc_set_parent(mem, parent);
    return mem;
}

/**
 * Allocate a (contiguous) memory chunk.
 *
 * @param size    amount of memory requested (in bytes).
 * @param parent  pointer to previously talloc'ed memory chunk from which this
 *                chunk depends, or NULL.
 *
 * @return pointer to the allocated memory chunk, or NULL if there was an error.
 */
void *talloc(size_t size, void *parent) {

    return talloc_init(malloc(size + HEADER_SIZE), parent);
}

/**
 * Allocate a zeroed (contiguous) memory chunk.
 *
 * @param size    amount of memory requested (in bytes).
 * @param parent  pointer to previously talloc'ed memory chunk from which this
 *                chunk depends, or NULL.
 *
 * @return pointer to the allocated memory chunk, or NULL if there was an error.
 */
void *tzalloc(size_t size, void *parent) {

    return talloc_init(calloc(1, size + HEADER_SIZE), parent);
}

/**
 * Modify the size of a talloc'ed memory chunk.
 *
 * @param usr   pointer to previously talloc'ed memory chunk.
 * @param size  amount of memory requested (in bytes).
 *
 * @return pointer to the allocated memory chunk, or NULL if there was an error.
 */
void *trealloc(void *usr, size_t size) {

    void *mem = realloc(usr ? usr2raw(usr) : NULL, size + HEADER_SIZE);

    if (!usr || !mem)
        return talloc_init(mem, NULL);

    mem = raw2usr(mem);

    /* If the buffer starting address changed, update all references. */

    if (mem != usr) {

        if (child(mem))
            parent(child(mem)) = mem;

        if (!is_root(mem)) {

            if (next(mem))
                prev(next(mem)) = mem;

            if (next(prev(mem)) == usr)
                next(prev(mem)) = mem;

            if (child(parent(mem)) == usr)
                child(parent(mem)) = mem;
        }
    }

    return mem;
}

/**
 * Deallocate all the descendants of parent(mem) recursively.
 *
 * @param mem  pointer to previously talloc'ed memory chunk.
 */
static void __tfree(void *mem) {

    if (!mem)
        return;

    /* Fail if the tree hierarchy has cycles. */

    assert(prev(mem));
    prev(mem) = NULL;

    __tfree(child(mem));
    __tfree(next(mem));
    free(usr2raw(mem));
}

/**
 * Deallocate a talloc'ed memory chunk and all the chunks depending on it.
 *
 * @param mem  pointer to previously talloc'ed memory chunk.
 *
 * @return always NULL, can be safely ignored.
 */
void *tfree(void *mem) {

    if (!mem)
        return NULL;

    talloc_set_parent(mem, NULL);

    __tfree(child(mem));
    free(usr2raw(mem));

    return NULL;
}

/**
 * Get the parent of a talloc'ed memory chunk (the chunk on which it depends).
 *
 * @param mem  pointer to previously talloc'ed memory chunk.
 *
 * @return pointer to the parent memory chunk (could be NULL).
 */
void *talloc_get_parent(void *mem) {

    if (!mem || is_root(mem))
        return NULL;

    while (!is_first(mem))
        mem = prev(mem);

    return parent(mem);
}

/**
 * Change the parent of a talloc'ed memory chunk. This will affect the
 * dependencies of the entire subtree rooted at the given chunk.
 *
 * @param mem     pointer to previously talloc'ed memory chunk.
 * @param parent  pointer to previously talloc'ed memory chunk from which this
 *                chunk depends, or NULL.
 */
void talloc_set_parent(void *mem, void *parent) {

    if (!mem)
        return;

    if (!is_root(mem)) {

        /* Remove node from old tree. */

        if (next(mem))
            prev(next(mem)) = prev(mem);

        if (!is_first(mem))
            next(prev(mem)) = next(mem);
        else
            child(parent(mem)) = next(mem);
    }

    next(mem) = prev(mem) = NULL;

    if (parent) {

        /* Insert node into new tree. */

        if (child(parent)) {
            next(mem) = child(parent);
            prev(child(parent)) = mem;
        }

        parent(mem) = parent;
        child(parent) = mem;
    }
}

/**
 * Remove a talloc'ed memory chunk from the dependency tree, taking care of its
 * children (they will depend on parent).
 *
 * @param mem     pointer to previously talloc'ed memory chunk.
 * @param parent  pointer to previously talloc'ed memory chunk from which this
 *                chunk's children will depend, or NULL.
 */
void talloc_steal(void *mem, void *parent) {

    if (!mem)
        return;

    talloc_set_parent(mem, NULL);

    if (!child(mem))
        return;

    if (parent) {

        /* Insert mem children in front of the list of parent children. */

        if (child(parent)) {

            void *last = child(mem);

            while (next(last))
                last = next(last);

            prev(child(parent)) = last;
            next(last) = child(parent);
        }

        child(parent) = child(mem);
    }

    parent(child(mem)) = parent;
    child(mem) = NULL;
}

