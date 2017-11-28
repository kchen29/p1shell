all: shell.o
	gcc shell.o

shell.o: shell.c
	gcc -c shell.c

test:
	./a.out < test.txt

clean:
	rm -f *o *~ a.out

run:
	./a.out
