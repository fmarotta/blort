/* blarr.c
 *
 * At ADT to deal with blocks of files. Each block is represented with a
 * 'Block' structure containing the key of the block (on which the file
 * will be sorted), its starting location in the file, and its ending
 * location. The blocks are contained into a dynamic array.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "blarr.h"

// Add large file support
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE 1

void InitializeArray(Array *a, size_t initial_size)
{
    a->array = (Block*) malloc(initial_size * sizeof(Block));
    if (a->array == NULL)
    {
        FreeArray(a);
        err(1, "malloc: %s", "InitializeArray");
        exit(EXIT_FAILURE);
    }
    a->used = 0;
    a->size = initial_size;
}

void AddItem(Array *a, char *key, unsigned long long start)
{
    Block pnew;

    // Increase the size of the array if we have reached capacity
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(Block));
        if (a->array == NULL)
        {
            FreeArray(a);
            err(1, "realloc: %s", "AddItem");
            exit(EXIT_FAILURE);
        }
    }

    // Add the new item
    strcpy(pnew.key, key);
    pnew.start = start;
    a->array[a->used] = pnew;

    // Set the end of the previous block
    if (a->used > 0)
        a->array[a->used - 1].end = start;

    a->used++;
}

void SetLastEnd(Array *a, unsigned long long end)
{
    if (a->used > 0)
        a->array[a->used - 1].end = end;
}

void FreeArray(Array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

bool ArrayIsEmpty(Array *a)
{
    return (a->used == 0);
}

char *LastKey(Array *a)
{
    return a->array[a->used - 1].key;
}
