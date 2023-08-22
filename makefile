INCLUDES = -Isrc/headers

LFLAGS = -g
CFLAGS = $(INCLUDES) -std=c++2a -g -c -Wall

bin/main: build/main.o build/graph.o | bin
	clang++ $(LFLAGS) -o $@ $^

build/%.o: src/%.cpp | build
	clang++ $(CFLAGS) -o $@ $^

bin:
	mkdir -p $@

build:
	mkdir -p $@

memtest: bin/main
	leaks -atExit -- bin/main demo_graphs/europe_graph.txt

europe_test: bin/main
	bin/main demo_graphs/europe_graph.txt

clean:
	rm -f build/* bin/*
