CXX = g++
CXXFLAGS = -Wall -fdiagnostics-color=always --std=c++17 -pedantic
SRC=$(wildcard src/**.cpp)
HEADERS=$(wildcard src/**.h)

.PHONY: run all

all: build/app

build/app: $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

build/debug: $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ -g3 $(SRC)

.SILENT: run
run: build/app
ifeq ($(input), )
	$^
else
	$^ < $(input)
endif