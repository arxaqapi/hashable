CFLAGS = -Wall -Wpedantic -Wextra -g # -Werror

run: build
	./ht_test && rm -f *.o

build: build prime.o hash_table.o main.o 
	gcc $^ -o ht_test -lm

hash_table.o: 
	gcc $(CFLAGS) -c src/hash_table.c

prime.o:
	gcc $(CFLAGS) -c src/prime.c

main.o:
	gcc $(CFLAGS) -c main.c -Isrc/

sweep:
	rm -f *.o

.PHONY: clean
clean:
	rm -f *.o ht_test

