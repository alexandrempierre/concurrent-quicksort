#include "insertion_sort.h"

/* Given two indices of an array and a reference to the array, sort the 
 * subarray delimited by them.
 * */
void insertion_sort (int lo, int hi, int v[]) {
    int i;

    /* for each element in the subarray checks whether it's in the right 
     * position and moves to the right all the elements that are greater 
     * than the current. 
     * */
    for (i = 1; i <= hi-lo; i++) {
        move_right(lo, i, v);
    }
}

/* Checks which elements to the left of the current one should be moved to 
 * the right (and moves). 
 * 
 * It takes the leftmost index of the interval, the current element index 
 * and the array. It makes the changes in-place, nothing is returned.
 * */
void move_right (int lo, int i, int v[]) {
    int j;
    int value = v[lo+i];

    for (j = lo+i; v[j-1] > value && j > lo; j--) {
         v[j] = v[j-1];
    }

    v[j] = value;
}