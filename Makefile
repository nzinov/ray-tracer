bin/main : main.cpp
	mkdir -p bin/
	g++ -std=c++14 -o $@ $^ `pkg-config --cflags --libs cairo x11`
