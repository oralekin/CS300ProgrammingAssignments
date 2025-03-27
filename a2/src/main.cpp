#include <functional>
#include <iostream>
#include <sstream>
using namespace std;

template <class T>
// coderunner runs on old c++, so quickly implement optional type
class Option {
	private:

	bool has;

	struct Container {
		T value;
		Container(T v) : value(v) {};
	};
	Container *inner;

	public:
	// none constructor
	Option() : has(false), inner(nullptr) {};
	// "some" constructor
	Option(T _value) : has(true), inner(new Container(_value)) {};
	// copy cons
	Option(const Option<T> &other) : has(other.has), inner(other.inner) {};
	// move cons
	Option(Option<T> &&other) {
		swap(inner, other.inner);
		swap(has, other.has);
	};

	const Option<T> &operator=(const Option<T> &other) {
		if (&other == this) return *this;

		// clear myself
		delete inner;
		has = other.has;
		inner = nullptr;

		// if other has a value:
		if (has) {
			// create container with copy of value from other.
			inner = new Container(other.inner->value);
		}

		return *this;
	}

	~Option() {
		delete inner;
		return;
	}

	constexpr T &operator*() const {
		return inner->value;
	};
	constexpr T operator->() const {
		return inner->value;
	};
	constexpr bool operator==(const Option<T> &other) const {
		return has && other.has && *inner->value == *other.inner->value;
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
			return f(inner->value);
		}
	}
	template <class U>
	Option<U> flatMap(function<Option<U>(T)> f) {
		if (!has) {
			return {};
		} else {
			return f(inner->value);
		}
	}
};

template<class Item>
// works for all items overloading operator== or operator!=
class HashTable {
	private:
	struct Slot {
		enum SlotStatus {
			// i cant think of a better name to use for
			// slots which have not yet been initialized...
			VIRGIN,
			INUSE,
			DELETED,
		};

		SlotStatus status;
		Item *value;
		Slot() : status(VIRGIN), value(nullptr) {};
		Slot(Item item) : status(INUSE), value(new Item(item)) {};
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

	// return None if table full, Some<pair<...>> otherwise
	// Some<pair<...,...>: spot this should be in, spot to insert into
	Option<pair<int, int>> findSpot(const Item &predicate) const {
		int init = hash(predicate) % capacity;
		int i = init;

		bool found_insert_spot = false;
		int insert_spot = i;

		/*
		stop when:
			- found an empty cell.
				- found a deleted cell with this predicate in it
				- found an active cell with this predicate in it
			- => found any cell with this predicate in it or is virgin
			=== continue if: !(virgin || predicate)
			=== continue if: (!virgin && !predicate)
		*/
		while (
			table[i].status != Slot::VIRGIN
			&& *(table[i].value) != predicate
			) {
			// if this spot is insertable and we havent found one yet:
			if (table[i].status != Slot::INUSE && !found_insert_spot) {
				insert_spot = i;
				found_insert_spot = true;
			}

			// progress i until we find it again.
			if (++i >= capacity) i -= capacity;
			// after progressing, if we are back at the start, there is no space.
			if (i == init) return {};
		}

		// if we don't find a good insert spot, default to i
		if (!found_insert_spot) insert_spot = i;

		return make_pair(i, insert_spot);
	};

	Option<const Item &> find(const Item &predicate) const {
		return
			findSpot(predicate)
			// (...).template map<...>(...) from https://stackoverflow.com/a/3786481
			.template map<int>([](pair<int, int> p) {return p.first;})
			.template flatMap<const Item &>([&](int idx) {
				const Slot &s = table[idx];
				Option<const Item &> r {};
				if (s.status == Slot::INUSE)
					r = Option<const Item &>(*(s.value));
				return r;
			});
	};

	// returns true if item was inserted
	// returns false if the item was inserted due to:
	// 	no space in the table
	// 	item already exists
	bool insert(Item item) {
		auto spot = findSpot(item);

		// no space left.
		if (!spot) return false;

		int loc, loc_insert;
		tie(loc, loc_insert) = *spot;

		// this value already exists in the table
		if (table[loc].status == Slot::INUSE) return false;

		// this value isnt in the table, so add it to the first empty spot we found.
		table[loc_insert] = Slot(item);

		return true;
	};

	// same as find but removes the item and returns it, if found
	Option<Item> remove(const Item &predicate) {
		return
			findSpot(predicate)
			.template map<int>([](pair<int, int> p) {return p.first;})
			// this is unfunctional because it modifies outside state... oops. 
			.template flatMap<Item>([&](int idx) {
				Slot &s = table[idx];
				Option<Item> r;
				if (s.status == Slot::INUSE) {
					r = Option((*(s.value)));
					s.status = Slot::DELETED;
				}
				return r;
			});
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
	StringCode(char c) : StringCode({ (char) c }, c) {};

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
		for (int c = 0; c < 256; c++)
			insert({ (char) c });
	};

	Option<const StringCode&> find(const string &str) const {
		return
			HashTable<StringCode>::find({ str, -1 });
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

	public:
	string compress(string s) {

		// index of the start of the uncompressed section
		unsigned int start = 0;
		stringstream result;

		while (start < s.length()) {
			// find the longest prefix in the uncompressed
			auto longest = longestCodeInTable(s.substr(start));
			result << longest.code << ' ';
			// we have compressed some part of the input.
			start += longest.str.length();
			// if theres a next character, add to map
			if (start < s.length())
				map.insert(longest.str + s[start]);
		}

		return result.str();
	}

	StringCode longestCodeInTable(string s) {
		for (unsigned int n = map.maxLength(); n > 0; n--) {
			if (n > s.length()) continue; // no substring of this size
			auto item = map.find(s.substr(0, n));
			if (item) return *item;
		}
		throw "shouldve found one-character code already!";
	}

};

class Decompressor {

	string dictionary[4096];
	stringstream output;

	public:

	Decompressor() {
		for (unsigned int c = 0; c < 256; c++)
			dictionary[c] = { (char) c };
	}

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
	os << d.output.str();

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
		string zip = c.compress(s);
		cout << "Compressed output: " << zip;
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