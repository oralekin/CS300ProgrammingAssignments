#include <iostream>
#include <sstream>
using namespace std;

template <class T>
// coderunner runs on old c++, so quickly implement optional type
class Option {
	private:

	bool has;

	struct V {
		T value;
	};
	V *vv;

	public:
	// default/none constructor
	Option() : has(false), vv(nullptr) {};
	// "some" constructor
	Option(T _value) : has(true), vv(new V) {
		vv->value = _value;
	};
	// copy cons
	Option(const Option<T> &other) : has(other.has), vv(other.vv) {};
	// move cons
	Option(Option<T> &&other) {
		swap(vv, other.vv);
		swap(has, other.has);
	};

	constexpr T &operator*() const {
		return vv->value;
	};
	constexpr T operator->() const {
		return vv->value;
	};
	constexpr bool operator==(const Option<T> &other) const {
		return has && other.has && *vv->value == *other.vv->value;
	}
	constexpr operator bool() const {
		return has;
	}

	// "monadic operations"
	template <class U>
	Option<U> map(function<U(T)> f) {
		if (!has) {
			return {};
		} else {
			return f(vv->value);
		}
	}
};

template<class Item>
// works for all items overloading operator== or operator!=
class HashTable {
	private:
	struct Slot {
		enum SlotStatus {
			VIRGIN,
			INUSE,
			DELETED,
		};

		SlotStatus status;
		Item *value;
		Slot() : status(VIRGIN), value(nullptr) {};
	};

	constexpr static float LOAD_LIMIT = .5;
	int capacity;
	Slot *table = nullptr;
	int _size = 0;

	// typedef unsigned long long(*HashFunction)(const Item &);
	typedef function<unsigned long long(const Item &)> HashFunction;
	HashFunction hash;
	// can't construct without associated hash function
	HashTable() = delete;

	public:
	HashTable(int capacity, HashFunction hash) : capacity(capacity), hash(hash) {
		table = new Slot[capacity];
	}

	// TODO: return None if table full, Some(pair<...>) otherwise
	// returns spot this should be in, and spot to insert into
	pair<int, int> findSpot(const Item &predicate) const {
		// TODO
		return { 0, 0 };
	};

	Option<Item &> find(const Item &predicate) const {
		// TODO
		return {};
	};

	bool insert(Item i) {
		// TODO

		return false;
	};

	Option<Item> remove(const Item &predicate) {
		// TODO
		return {};
	};

	int size() const {
		return _size;
	};
};

// ---------------- specifics

struct StringCode {
	string str;
	int code;

	StringCode(string str, int code) : str(str), code(code) {};
	StringCode(char c) : StringCode({ c }, c) {};

	bool operator==(const StringCode &other) const {
		return str == other.str;
	}

	bool operator!=(const StringCode &other) const {
		return !operator==(other);
	}

	// simple function from the book
	static unsigned long long hash(const StringCode &sc) {
		unsigned long long result = 0;
		for (char ch : sc.str)
			result = (37 * result) + ch;
		return result;
	}
};

class CodeTable : public HashTable<StringCode> {
	int codeCount = 0;
	// length of the longest hashed string in the table
	int _maxLength = 1;

	public:
	CodeTable() : HashTable(4096, &StringCode::hash) {
		for (char c = 0; c < 256; c++)
			insert({ c });
	};

	// dropped reference because it doesn't work with Option::map
	Option<int> find(const string &str) const {
		return
			HashTable<StringCode>::find({ str, -1 })
			.map<int>([](const StringCode &sc) {return sc.code;});
	};

	bool insert(const string &str) {
		if (HashTable::insert({ str, codeCount })) {
			codeCount++;
			_maxLength = max((int)str.length(), _maxLength);
			return true;
		} else return false;
	}

	int maxLength() const {
		return _maxLength;
	}
};

class Compressor {
	CodeTable map;
	// last stringcode that was inserted
	// use some placeholder value, won't be read before it is written to.
	StringCode last = StringCode("\0", -1);

	string compress(string s) {

		// index of the start of the uncompressed section
		unsigned int start = 0;
		stringstream result;

		while (start < s.length()) {
			// find the longest prefix in the uncompressed
			auto longest = longestCodeInTable(s.substr(start));
			result << printf("%d ", longest.code);
			// we have compressed some part of the input.
			start += longest.str.length();
			// if theres a next character, add to map
			if (start < s.length())
				map.insert(longest.str + s[start]);
		}

		return result.str();
	}

	StringCode longestCodeInTable(string s) {
		for (int n = map.maxLength(); n > 0; n--) {
			if (n > s.length()) continue; // no substring of this size
			auto item = map.find(s.substr(0, n));
			if (item) return *item;
		}
		throw "shouldve found one-character code already!";
	}

};


class Decompressor {


	public:
	// desired usage:
	/*
	Decompressor d;
	while (d << cin);
	cout << output;
	*/
	bool operator<<(istream &is) {
		// TODO

		return false;
	};
	friend ostream &operator<<(ostream &os, const Decompressor &d);
};

ostream &operator<<(ostream &os, const Decompressor &d) {
	// TODO

	return os;
}



int main() {
	cout << "To compress a file, press 1. To decompress a file, press 2: ";

	// because getline and >> dont work together 
	string choice_str;
	getline(cin, choice_str);
	int choice = atoi(choice_str.c_str());

	cout << "Enter the input string: ";
	string s;
	getline(cin, s);

	switch (choice) {
	case 1:
	{ // compressing
		Compressor c;
		// string zip = c.compress(s);
		// cout << zip << endl;
	}
	break;
	case 2:
	{ // decompressing
		Decompressor d;
		stringstream ss(s);
		while (d << ss);
		cout << d;
	}
	break;

	default:
		cout << "Bad input." << endl;
		return 1;
		break;
	}

	return 0;
}