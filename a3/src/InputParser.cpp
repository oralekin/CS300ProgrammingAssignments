#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "InputParser.h"

using namespace std;

Command parseOne(istream &is) {
	is >> ws;	 // remove any leading whitespace
	string temp;
	if (!getline(is, temp)) throw EndOfFile();
	stringstream line(temp);

	string command_string;
	line >> command_string;

#pragma region huge if chain to parse command
	// check insert first because its more likely to appear, probably
	if (command_string == "INSERT") {
		int task_id, priority;
		if (!(line >> task_id) || !(line >> priority)) {
			throw UnrecognizedCommand(line.str());
		}
		return Insert{ task_id, priority };
	} else if (command_string == "DECREASE_KEY") {
		int task_id, new_priority;
		if (!(line >> task_id) || !(line >> new_priority))
			throw UnrecognizedCommand(line.str());
		return DecreaseKey{ task_id, new_priority };
	} else if (command_string == "EXTRACT_MIN") {
		return ExtractMin();
	} else if (command_string == "MERGE") {
		string filename;
		if (!(line >> filename)) throw UnrecognizedCommand(line.str());
		return Merge{ filename };
	} else if (command_string == "PRINT") {
		return Print();
	} else if (command_string == "HEAP_TYPE") {
		string type;
		if (!(line >> type)) throw UnrecognizedCommand(line.str());
		if (type == "BINOMIAL") return Binomial();
		else if (type == "DARY") {
			size_t d;
			if (!(line >> d)) throw UnrecognizedCommand(line.str());
			return Dary{ d };
		}
	}
#pragma endregion

	// if we didnt match anything:
	throw UnrecognizedCommand(line.str());
}

vector<Command> readFile(filesystem::path filename) {
	// run through file until we find eof
	vector<Command> commands;
	ifstream file(filename);	// open file

	try {
		while (true) {
			auto command = parseOne(file);
			commands.push_back(command);
		}
	} catch (EndOfFile &e) {}	 // ignore eof
	return commands;
}