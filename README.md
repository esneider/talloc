# talloc
[![Build Status](https://travis-ci.org/esneider/talloc.svg?branch=master)](https://travis-ci.org/esneider/talloc)
[![Coverage Status](https://img.shields.io/coveralls/esneider/talloc.svg)](https://coveralls.io/r/esneider/talloc?branch=master)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/2438/badge.svg)](https://scan.coverity.com/projects/2438)

Talloc is a hierarchical memory allocator. That means that it is similar to
*malloc*, but it can also track the natural tree-like structure of memory
dependencies. Freeing a *talloc-ed* chunk of memory will free all of its
dependencies.

## Example

As an example we develop the allocation operations for a bi-dimensional matrix
of integers. For simplicity, the example does not handle out-of-memory errors.

```c
struct matrix {
    size_t rows;
    size_t cols;
    int **data;
};

struct matrix *new_matrix(size_t rows, size_t cols) {

    struct matrix *m = tzalloc(sizeof(*m), NULL);

    m->rows = rows;
    m->cols = cols;
    m->data = tzalloc(rows * sizeof(*m->data), m);

    for (size_t i = 0; i < rows; i++)
        m->data[i] = talloc(cols * sizeof(**m->data), m->data);

    return m;
}

void free_matrix(struct matrix *m) {

    tfree(m);
}
```

## API

The API is really simple and each function does what you expect. For a detailed
explanation of each function, [see the inline
documentation](https://github.com/esneider/talloc/blob/master/src/talloc.h).

The only detail is that you cannot mix the *malloc* and *talloc* families of
functions for a given chunk of memory. Once a chunk is allocated with *talloc*
(*malloc*), it can only be freed with *tfree* (*free*).

### Basic

```c
void *talloc(size_t size, void *parent);

void *tzalloc(size_t size, void *parent);

void *trealloc(void *mem, size_t size);

void *tfree(void *mem);
```

### Advanced

```c
/**
 * Get the parent of a talloc'ed memory chunk (the chunk on which it depends).
 */
void *talloc_get_parent(void *mem);

/**
 * Change the parent of a talloc'ed memory chunk. This will affect the
 * dependencies of the entire subtree rooted at the given chunk.
 */
void talloc_set_parent(void *mem, void *parent);

/**
 * Remove a talloc'ed memory chunk from the dependency tree, taking care of its
 * children (they now depend on parent).
 */
void talloc_steal(void *mem, void *parent);
```

## Overhead

For each *talloc'ed* chunk of memory, 3 extra pointers are used to maintain the
internal structure. The time overhead for using *talloc* is quite small, since
([as you can see](https://github.com/esneider/talloc/blob/master/src/talloc.c)) the
code is extremely simple.

## Credits

This library was created in an attempt to simplify the awesome but
unnecessarily bloated [Samba talloc](http://talloc.samba.org/).

## Relevant

![http://xkcd.com/835/](http://imgs.xkcd.com/comics/tree.png "Not only is that terrible in general, but you just KNOW Billy's going to open the root present first, and then everyone will have to wait while the heap is rebuilt.")

Source: [xkcd](http://xkcd.com/835/)

