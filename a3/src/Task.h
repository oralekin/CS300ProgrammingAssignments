#pragma once
#include <ostream>
#include <string>

class Task {
	public:
	int task_id;
	int priority;
	std::string location;
	std::string category;

	Task(int id, int prio, std::string loc = "", std::string cat = "");

	// comparison operators should compare priority
	bool operator<(const Task &rhs) const;
	bool operator>(const Task &rhs) const;
	bool operator>=(const Task &rhs) const;
	bool operator<=(const Task &rhs) const;

	friend std::ostream &operator<<(std::ostream &, const Task &);
};
