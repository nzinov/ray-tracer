bin/main : main.cpp *.hpp Makefile
	mkdir -p bin/
	clang++ -g -std=c++14 -O3 -o $@ main.cpp `pkg-config --cflags --libs cairo x11`
test : bin/main
	bin/main
