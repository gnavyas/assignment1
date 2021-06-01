CXX=c++
CXXFLAGS=-Wextra -Wall

all: client server

client: clientmain.cpp
	$(CXX) $(CXXFLAGS) -o client clientmain.cpp

server: servermain.cpp
	$(CXX) $(CXXFLAGS) -o server servermain.cpp

clean:
	rm -f client server
