#pragma once
#include <string>

class Task {
	public:
	int task_id;
	int priority;
	std::string location;
	std::string category;
	Task(int id, int prio, std::string loc = "", std::string cat = "");
};