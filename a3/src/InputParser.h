#pragma once
#include <filesystem>
#include <fstream>
#include <istream>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

using namespace std;

// clang-format off

// helpers for working with variant<>
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

struct Binomial {};
struct Dary {
  int d;
};
// HEAP_TYPE BINOMIAL
// HEAP_TYPE DARY <d>
using HeapType = variant<Binomial, Dary>;

// INSERT <task_id> <priority>
struct Insert {
	int taskId;
	int priority;
};

// DECREASE_KEY <task_id> <new_priority>
struct DecreaseKey {
	int taskId;
	int newPriority;
};

// EXTRACT_MIN
struct ExtractMin { monostate _; };

// MERGE <filename>
struct Merge {
	string filename;
};

// PRINT
struct Print { monostate _; };

using Command =
	variant<
    HeapType,
    Insert, 
    DecreaseKey, 
    ExtractMin, 
    Merge, 
    Print
  >;

struct Parser {};

struct UnrecognizedCommand { string command; UnrecognizedCommand() = delete; UnrecognizedCommand(string str) : command(str) {};};
struct EndOfFile {monostate _;};

// clang-format on

Command parseOne(istream &is);

vector<Command> readFile(filesystem::path filename);