# blort

A sorting algorithm optimised for files made of 'blocks'. A block consists of
all the contiguous rows that have the same value in the first field.

This program efficiently sorts files where there are several consecutive lines
with the same key. Each block of lines with the same key is considered as a
single item as far as the sorting is concerned. This can dramatically reduce
the number of comparisons, as well as the I/O operations, with respect to GNU
sort, thereby speeding up the process. The drawback is that it is not possible
to sort piped files.

The time complexity of the algorithm is `O(N + M log(M))` where N is the
size of the file and M is the number of different blocks. Usually `M <<
N.`

## Compilation

From the root directory of the repository, compile the program with

```
gcc blort.c blarr.c -o blort
```

(Optional) make it executable with

```
chmod u+x blort
```

## Execution

```
blort <SOURCE> [<TARGET>]
```

where:

* _SOURCE_ is the file to be sorted; it should have a block structure,
where the first field is the same for a lot of consecutive lines before
changing. The first field is also the key for the sorting.

* _TARGET_ is the name of the output file. If omitted, the sorted file
is printed directly to standard output.

NOTE that the blort executable should be either in the working directory
or in a directory listed in the PATH environmental variable. Otherwise,
specify the full path to it.

## Bug reports

federicomarotta AT mail DOT com
