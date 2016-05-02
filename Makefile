bin/main : main.cpp *.hpp Makefile
	mkdir -p bin/
	clang++ ${CXXFLAGS} -std=c++14 -O4 -o $@ main.cpp `pkg-config --cflags --libs cairo x11`
test : bin/main
	bin/main
