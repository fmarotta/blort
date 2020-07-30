#include <stdbool.h>

#define KEYSIZE 80

typedef struct {
    char key[KEYSIZE];
    long start;
    long end;
} Block;

typedef struct {
    Block *array;
    size_t used;
    size_t size;
} Array;

void InitializeArray(Array *a, size_t initial_size);

bool ArrayIsEmpty(Array *a);

char *LastKey(Array *a);

void AddItem(Array *a, char *key, long start);

void SetLastEnd(Array *a, long end);

// void Traverse(const Array *a, void (*pfun)(Block b));

void FreeArray(Array *a);
