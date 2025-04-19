CXX = g++
CXXFLAGS = -Wall -fdiagnostics-color=always --std=c++17
SRC=$(wildcard src/**.cpp)
HDR=$(wildcard src/**.h)

.PHONY: run all

all: build/app

build/app: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

build/debug: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $@ -g3 $(SRC)

.SILENT: run
run: build/app
ifeq ($(input), )
	$^
else
	$^ < $(input)
endif