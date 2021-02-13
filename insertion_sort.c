#include "insertion_sort.h"

void insertion_sort (int lo, int hi, int v[]) {
    int i;

    for (i = 1; i <= hi-lo; i++) {
        move_right(lo, i, v);
    }
}

void move_right (int lo, int i, int v[]) {
    int j;
    int value = v[lo+i];

    for (j = lo+i; v[j-1] > value && j > lo; j--) {
         v[j] = v[j-1];
    }

    v[j] = value;
}