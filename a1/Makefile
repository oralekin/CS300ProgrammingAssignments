CXX = g++
CXXFLAGS = -Wall -fdiagnostics-color=always
SRC=$(wildcard src/**.cpp)

.PHONY: run

build/app: $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

build/debug: $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ -g3 $(SRC)

.SILENT: run
run: build/app
ifeq ($(input), )
	echo "no input, keeping stdin"
	$^
else
	$^ < $(input)
endif