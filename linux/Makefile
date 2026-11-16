
CXX = g++
GMPXXLIBS := $(shell pkg-config --libs gmpxx)
ifeq ($(strip $(GMPXXLIBS)),)
	GMPXXLIBS = -lgmpxx -lgmp
endif
CXXFLAGS = $(GMPXXLIBS) -std=c++20
SRCS = polynomial.cpp modint.cpp
HDRS = $(SRCS:.cpp=.hpp)
.PHONY: all main test clean

all: crt-rsa_attack unit_test
main: crt-rsa_attack
test: unit_test

crt-rsa_attack: main.cpp $(SRCS) $(HDRS)
	$(CXX) $(SRCS) main.cpp -o $@ $(CXXFLAGS)

unit_test: unit_test.cpp $(SRCS) $(HDRS)
	$(CXX) $(SRCS) unit_test.cpp -o $@ $(CXXFLAGS)

clean:
	rm -f crt-rsa_attack unit_test
