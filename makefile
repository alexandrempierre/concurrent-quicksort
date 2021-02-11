all: clean random_stream.out quicksort.out

quicksort.out: queue.o
	gcc -o quicksort.out quicksort.c queue.o -lpthread -Wall --pedantic

random_stream.out: random_stream.c
	gcc -o random_stream.out random_stream.c -Wall --pedantic

queue.o: queue.c
	gcc -o queue.o queue.c -c -Wall --pedantic

clean:
	rm *.o *.out