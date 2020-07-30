/* blort.c
 *
 * This program efficiently sorts files where there are several
 * consecutive lines with the same key. Each block of lines with the
 * same key is considered as a single item as far as the sorting is
 * concerned. This can dramatically reduce the number of comparisons, as
 * well as the I/O operations with respect to GNU sort, thereby speeding
 * up the process. The drawback is that it is not possible to sort
 * piped files.
 *
 * Compile with blarr.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "blarr.h"

#define BUFSIZE 4096

void append(FILE *source, FILE*dest, long start, long n);
int blockcmp(const void *p1, const void *p2);

int main(int argc, char *argv[])
{
    FILE *in, *out;
    char c;

    Array blocks;
    char key[KEYSIZE];

    if (argc == 2) {
        if ((in = fopen(argv[1], "rb")) == NULL)
        {
            err(1, "fopen: %s", argv[1]);
            exit(EXIT_FAILURE);
        }
        if (setvbuf(in, NULL, _IOFBF, BUFSIZE) != 0)
        {
            err(1, "setvbuf: %s", argv[1]);
            exit(EXIT_FAILURE);
        }

        out = stdout;
    }
    else if (argc == 3)
    {
        if ((in = fopen(argv[1], "rb")) == NULL)
        {
            err(1, "fopen: %s", argv[1]);
            exit(EXIT_FAILURE);
        }
        if (setvbuf(in, NULL, _IOFBF, BUFSIZE) != 0)
        {
            err(1, "setvbuf: %s", argv[1]);
            exit(EXIT_FAILURE);
        }

        if ((out = fopen(argv[2], "ab")) == NULL)
        {
            err(1, "fopen: %s", argv[2]);
            exit(EXIT_FAILURE);
        }
        if (setvbuf(out, NULL, _IOFBF, BUFSIZE) != 0)
        {
            err(1, "setvbuf: %s", argv[2]);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "Usage: %s IN_FILE [OUT_FILE]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    InitializeArray(&blocks, 128);

    // Scan the file once from beginning to end; when encountering a new
    // block, add its coordinates (key, start, end) to the blocks array.
    // The end is automatically added once the next block is read,
    // except for the last block which needs special handling (see the
    // SetLastEnd() function).
    while (1)
    {
        fscanf(in, "%80s", key);

        if (ArrayIsEmpty(&blocks))
            AddItem(&blocks, key, 0L);
        else if (strcmp(key, LastKey(&blocks)))
            AddItem(&blocks, key, ftell(in) - strlen(key));

        while ((c = getc(in)) != '\n' & c != EOF) continue;
        if (c == EOF) break;
    }
    SetLastEnd(&blocks, ftell(in));

    // Sort the array of blocks.
    qsort(blocks.array, blocks.used, sizeof(Block), blockcmp);

    // Randomly access the file in the correct order, and print the
    // blocks to the output file.
    for (int i = 0; i < blocks.used; i++)
        append(in, out, blocks.array[i].start, blocks.array[i].end - blocks.array[i].start);

    // Final dispositions.
    FreeArray(&blocks);
    fclose(in);
    fclose(out);
    return 1;
}

int blockcmp(const void *p1, const void *p2)
{
    const Block *a1 = (const Block *) p1;
    const Block *a2 = (const Block *) p2;

    return strcmp(a1->key, a2->key);
}

void append(FILE *source, FILE*dest, long start, long n)
{
    size_t bytes;
    char block[n];

    fseek(source, start, SEEK_SET);
    bytes = fread(block, n, 1, source);
    fwrite(block, n, 1, dest);
}
