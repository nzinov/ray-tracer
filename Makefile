all : bin/debug bin/main

bin/debug : main.cpp *.hpp Makefile
	mkdir -p bin/
	clang++ ${CXXFLAGS} -Wall -march=native -std=c++14 -g -o $@ main.cpp `pkg-config --cflags --libs cairo x11` -pthread

bin/main : main.cpp *.hpp Makefile
	mkdir -p bin/
	clang++ ${CXXFLAGS} -Wall -march=native -std=c++14 -Ofast -o $@ main.cpp -fopenmp `pkg-config --cflags --libs cairo x11` -pthread
test : bin/main
	bin/main
