build:
	cc -std=c++20 -c ./src/ext/*.cpp -o ./bin/jellyfish.o
	ar rcs ./bin/jellyfish.a ./bin/*.o

clean:
	rm -rf ./bin/**