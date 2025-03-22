CXX = g++
CXXFLAGS = -Wall -fdiagnostics-color=always --std=c++14
SRC=$(wildcard src/**.cpp)

.PHONY: run all

all: build/app

build/app: $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

build/debug: $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ -g3 $(SRC)

build/submission: submission/code.cpp
	$(CXX) $(CXXFLAGS) --std=c++14 -o $@ $(SRC)

.SILENT: run
run: build/app
ifeq ($(input), )
	$^
else
	$^ < $(input)
endif