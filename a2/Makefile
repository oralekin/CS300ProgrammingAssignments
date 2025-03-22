CXX = g++
CXXFLAGS = -Wall -fdiagnostics-color=always -pedantic
SRC=$(wildcard src/**.cpp)

.PHONY: run all

all: build/app

build/app: $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

build/debug: $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ -g3 $(SRC)

.SILENT: run
run: build/app
ifeq ($(input), )
	$^
else
	$^ < $(input)
endif