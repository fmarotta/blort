#include <stdbool.h>

#define KEYSIZE 80

// Add large file support
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE 1

typedef struct {
    char key[KEYSIZE];
    unsigned long long start;
    unsigned long long end;
} Block;

typedef struct {
    Block *array;
    size_t used;
    size_t size;
} Array;

void InitializeArray(Array *a, size_t initial_size);

bool ArrayIsEmpty(Array *a);

char *LastKey(Array *a);

void AddItem(Array *a, char *key, unsigned long long start);

void SetLastEnd(Array *a, unsigned long long end);

// void Traverse(const Array *a, void (*pfun)(Block b));

void FreeArray(Array *a);
