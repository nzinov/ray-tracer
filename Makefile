bin/main : main.cpp *.hpp Makefile
	mkdir -p bin/
	clang++ ${CXXFLAGS} -march=native -std=c++14 -Ofast -o $@ main.cpp -fopenmp `pkg-config --cflags --libs cairo x11`
test : bin/main
	bin/main
