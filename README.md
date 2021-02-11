# concurrent-quicksort

Using the `random_stream.out` program we can generate any number of random values to feed the `quicksort.out` program.

## Foreword

I don't intend to make a fully portable fully general purpose concurrent quicksort with the most general queue implementation. Everything is very specific to the assignment for which this project is the answer.

## Technologies in this project

* GCC
* POSIX Threads
* GNU make

## Compiling the code

Just opening the project directory in a terminal and typing `make all` then hitting Enter should be enough to have executable of the programs.

## Calling `random_stream.out` program

`./random_stream.out <seed> <upper_bound>`

The first command line argument will always be the seed and the second will be the upper bound of the generated random numbers. Upper bound means that the random numbers generated will be at least 0 and lower then `upper_bound`, `upper_bound` won't ever be generated. It's not necessary to have command line arguments and the program works with only one too.

## `quicksort`

`./quicksort.out <number of threads> <length of the array to be sorted>`

The elements of the array will be read through `scanf`, with pipe redirection the `random_stream` can feed this `scanf` calls.

There's a piece of code at the end of `quicksort` 's `main` to print the sorted array, but to not have huge prints distorting the time mesurements I'll leave it commented. I'll only keep a single line to make clear that the array is sorted. If you're reading this, just know that the print is working.

## `test/` directory

I intend to store the shorter test inputs in this directory, the ones already there are mostly for testing correctness.