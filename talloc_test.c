#if 0
#!/bin/bash
gcc -ansi -pedantic -Wall -Wextra talloc.c -g talloc_test.c -o talloc_test
valgrind ./talloc_test
rm talloc_test
exit
#endif

#include "talloc.h"


int main(void) {

    char *a = talloc(3243, NULL);
    char *b = talloc(3243, a);
    char *c = talloc(3243, a);
    char *d = talloc(3243, b);
    char *e = talloc(3243, d);
    char *f = talloc(3243, d);

    char *g = talloc(3243, NULL);
    char *h = talloc(3243, g);
    char *i = talloc(3243, h);
    char *j = talloc(3243, h);
    char *k = talloc(3243, j);

    c = trealloc(c, 12345);
    d = trealloc(d, 12345);
    e = trealloc(e, 12345);

    g = trealloc(g, 12345);
    j = trealloc(j, 12345);
    j = trealloc(j, 12345);
    i = trealloc(i, 12345);

    talloc_steal(h, g); /* same as talloc_steal( h, talloc_get_parent(h) ); */

    tfree(a);
    tfree(j);
    tfree(g);

    ((int*)h)[123] = 456;
    tfree(h);

    (void)f;
    (void)k;

    return 0;
}

