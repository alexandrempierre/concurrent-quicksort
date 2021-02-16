all: ones.out random_stream.out quicksort.out

quicksort.out: queue.o insertion_sort.o lcg.o
	gcc -o quicksort.out quicksort.c queue.o insertion_sort.o lcg.o -lpthread -Wall --pedantic

random_stream.out: random_stream.c lcg.o
	gcc -o random_stream.out random_stream.c lcg.o -Wall --pedantic

ones.out: ones.c
	gcc -o ones.out ones.c -Wall --pedantic

queue.o: queue.c
	gcc -o queue.o queue.c -c -Wall --pedantic

insertion_sort.o: insertion_sort.c
	gcc -o insertion_sort.o insertion_sort.c -c -Wall --pedantic

lcg.o: lcg.c
	gcc -o lcg.o lcg.c -c -Wall --pedantic

clean:
	rm *.o *.out