all:
	g++ addrbook.cpp -o addrbook
	g++ socialnetwork.cpp -o socialnetwork
	g++ library.cpp -o library
	g++ sortbench.cpp -o sortbench -std=c++17 -O3
	g++ randgen.cpp -o randgen