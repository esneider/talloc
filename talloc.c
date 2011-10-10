/**
 * @file talloc.c
 *
 * @brief Provides structure aware allocations
 *
 * @author Dario Sneidermanis
 */

#include <stdlib.h>
#include "talloc.h"


/**
 * Allocate memory
 *
 * @param size    amount of memory requested
 * @param parent  pointer to previously talloc'ed memory from wich this chunk
 *                depends or NULL
 *
 * @return a pointer to the allocated memory
 */
void* talloc ( size_t size, void* parent ) {

	void **mem = malloc( size + sizeof( void* ) * 3 );
	if ( !mem ) return NULL;

	mem[0] = mem[1] = mem[2] = NULL;

	talloc_set_parent( mem + 3, parent );

	return mem + 3;
}


/**
 * Allocate zeroed memory
 *
 * @param size    amount of memory requested
 * @param parent  pointer to previously talloc'ed memory from wich this chunk
 *                depends or NULL
 *
 * @return a pointer to the allocated memory
 */
void* tcalloc ( size_t size, void* parent ) {

	void **mem = calloc( 1, size + sizeof( void* ) * 3 );
	if ( !mem ) return NULL;

	talloc_set_parent( mem + 3, parent );

	return mem + 3;
}


/**
 * Re-allocate memory
 *
 * @param mem   pointer to previously talloc'ed memory
 * @param size  amount of memory requested
 *
 * @return a pointer to the allocated memory if successful, NULL otherwise
 */
void* trealloc ( void* mem, size_t size ) {

	void **header[3], **aux;

	if ( !mem ) return talloc( size, NULL );

	header[0] = ((void**)mem)[-3];
	header[1] = ((void**)mem)[-2];
	header[2] = ((void**)mem)[-1];

	aux = realloc( (void**)mem - 3, size + sizeof( void* ) * 3 );
	if ( !aux ) return NULL;

	if ( (void**)mem - 3 != aux ) {

		aux[0] = header[0];
		aux[1] = header[1];
		aux[2] = header[2];
		if ( aux[0] ) header[0][2] = aux;
		if ( aux[1] ) header[1][2] = aux;
		if ( aux[2] ) header[2][ header[2][1] == (void**)mem - 3 ] = aux;
	}

	return aux + 3;
}


/**
 * This is for internal use
 *
 * @see tfree
 */
static void __tfree ( void** mem ) {

	if ( !mem ) return;

	__tfree( mem[0] );
	__tfree( mem[1] );
	free( mem );
}


/**
 * Free memory
 *
 * @param mem  pointer to previously talloc'ed memory
 */
void tfree ( void* mem ) {

	if ( !mem ) return;

	talloc_set_parent( mem, NULL );

	__tfree( ((void**)mem)[-3] );

	free( (void**)mem - 3 );
}


/**
 * Get parent of talloc'ed memory
 *
 * @param mem  pointer to previously talloc'ed memory
 *
 * @return pointer to previously talloc'ed memory from which this chunk depends
 */
void* talloc_get_parent ( void* mem ) {

	void*** i;

	if ( !mem || !((void**)mem)[-1] ) return NULL;

	for ( i = (void***)mem - 3; i[2][1] == i; i = (void***)(i[2]) ) ;

	return i[2] + 3;
}


/**
 * Set parent of talloc'ed memory
 *
 * @param mem     pointer to previously talloc'ed memory
 * @param parent  pointer to previously talloc'ed memory from wich this chunk
 *                depends or NULL
 */
void talloc_set_parent ( void* mem, void* parent ) {

	if ( !mem ) return;

	if ( ((void**)mem)[-1] ) {

		if ( ((void**)mem)[-2] ) ((void***)mem)[-2][2] = ((void**)mem)[-1];

		((void***)mem)[-1][ ((void***)mem)[-1][1] == (void**)mem - 3 ] =
			((void**)mem)[-2];
	}

	if ( parent ) {

		((void**)mem)[-2] = ((void**)parent)[-3];
		((void**)mem)[-1] = (void**)parent - 3;
		((void**)parent)[-3] = (void**)mem - 3;

		if ( ((void**)mem)[-2] )
			((void***)mem)[-2][2] = (void**)mem - 3;
	} else
		((void**)mem)[-1] = ((void**)mem)[-2] = NULL;
}


/**
 * Remove chunk of talloc'ed memory from dependency chain.
 *
 * @param mem     pointer to previously talloc'ed memory
 * @param parent  pointer to previously talloc'ed memory from wich this chunk's
 *                sons will depend or NULL
 */
void talloc_steal ( void* mem, void* parent ) {

	void **aux;

	if ( !mem ) return;

	for ( aux = ((void***)mem)[-3]; aux; aux = ((void***)mem)[-3] ) {

		aux[2] = NULL;

		((void**)mem)[-3] = aux[1];

		talloc_set_parent( aux + 3, parent );
	}

	talloc_set_parent( mem, NULL );
}

