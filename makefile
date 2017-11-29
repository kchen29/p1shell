all: shell.o
	gcc shell.o

shell.o: shell.c
	gcc -c shell.c

test: all
	./a.out < test.txt

clean:
	rm -f *o *~ a.out

run: all
	./a.out
