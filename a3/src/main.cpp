#include <iostream>
#include "InputParser.h"

using namespace std;

// TODO: merge dependency graph: allow different files to merge with the same other file, but detect cycles in dependency.
// TODO: lazy running: analyze input files for dependencies before doing ops

int main() {
  cout << "Hello World!" << endl;
  try {auto comms = readFile("/home/oralekin/Code/sabanci/cs300/a/a3/samples/sample.in");
  } catch (UnrecognizedCommand uc) {
    cout << "err: unrecgonized " << quoted(uc.command) << endl;
  }
  cout << "done reading" << endl;
}