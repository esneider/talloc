# talloc

Talloc is a hierarchical memory allocator. That means that it is similar to
*malloc*, but it can also track the natural tree-like structure of memory
dependencies. Freeing a *talloc-ed* chunk of memory will free all of its
dependencies.

## Example

As an example we develop the allocation operations for a bidimensional matrix
of integers. For simplicity, the example does not handle out-of-memory errors.

```c
struct matrix {
    size_t rows
    size_t cols;
    int **data;
};

struct matrix *new_matrix(size_t rows, size_t cols) {

    struct matrix *m = talloc(sizeof(*m), NULL);

    m->rows = rows;
    m->cols = cols;
    m->data = talloc(rows * sizeof(*m->data), m);

    while (rows--) {
        m->data[rows] = talloc(cols * sizeof(**m->data), m->data);
    }

    return m;
}

void free_matrix(struct matrix *m) {

    tfree(m);
}
```

## API

The API is really simple and each function does what you expect. For a detailed
explanation of each function, [see the inline
documentation](https://github.com/esneider/talloc/blob/master/talloc.h).

The only detail is that you cannot mix the *malloc* and *talloc* families of
functions for a given chunk. Once a chunk of memory is allocated with *talloc*
(*malloc*), it can only be freed with *tfree* (*free*).

### Basic

```c
void* talloc(size_t size, void* parent);

void* tcalloc(size_t size, void* parent);

void* trealloc(void* mem, size_t size);

void tfree(void* mem);
```

### Advanced

```c
/* Get the chunk of memory on which mem depends (its parent) */
void* talloc_get_parent(void* mem);

/* Change the parent for the talloc-ed chunk mem */
void talloc_set_parent(void* mem, void* parent);

/* Extract the chunk mem from the dependency tree, and set its children to
   depend on parent */
void talloc_steal(void* mem, void* parent);
```

## Overhead

For each *talloc-ed* chunk, 3 extra pointers will be used to maintain the
internal structure. The time overhead for using *talloc* is quite small, since
([as you can see](https://github.com/esneider/talloc/blob/master/talloc.c)) the
code is extremely simple.

## Credits

This library was created in an attempt to simplify the awesome but
unnecessarily bloated [Samba Talloc](http://talloc.samba.org/).

## Relevant

![http://xkcd.com/835/](http://imgs.xkcd.com/comics/tree.png "Not only is that terrible in general, but you just KNOW Billy's going to open the root present first, and then everyone will have to wait while the heap is rebuilt.")

Source: [xkcd](http://xkcd.com/835/)
