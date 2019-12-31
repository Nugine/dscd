all:
	g++ addrbook.cpp -o addrbook
	g++ socialnetwork.cpp -o socialnetwork
	g++ library.cpp -o library
	g++ varsort.cpp -o varsort -std=c++17 -O3