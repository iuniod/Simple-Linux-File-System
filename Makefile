all: build

build:
	gcc -std=gnu99 -g main.c tree.c utils.c -o sfls

clean:
	rm *.o sfls

run:
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./sfls
