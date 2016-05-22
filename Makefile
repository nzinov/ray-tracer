all : bin/debug bin/main

bin/debug : main.cpp *.hpp Makefile
	mkdir -p bin/
	clang++ ${CXXFLAGS} -Wall -march=native -std=c++14 -g -o $@ main.cpp -fopenmp `pkg-config --cflags --libs cairo x11`

bin/main : main.cpp *.hpp Makefile
	mkdir -p bin/
	clang++ ${CXXFLAGS} -Wall -march=native -std=c++14 -Ofast -o $@ main.cpp -fopenmp `pkg-config --cflags --libs cairo x11`
test : bin/main
	bin/main
