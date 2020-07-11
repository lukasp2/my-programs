#include "hash.h"

#include <fstream>

using namespace std;

// gets the hash
string Hash::get() {
    string s{};

    for (int i : ascii_values) {
	s += static_cast<char>(i);
    }
    
    return s;
}

Hash& Hash::operator++() {
    if (ascii_values.size() == 0) {
	ascii_values.push_back(0);
    }
    else {
	incr_pos(0);
    }
    
    return *this;
}

// helper for op++();
void Hash::incr_pos(int idx) {
    if (ascii_values.at(idx) == 255) {
	ascii_values.at(idx) = 0;
	if (idx != static_cast<int>(ascii_values.size() - 1)) {
	    incr_pos(idx+1);
	}
	else {
	    ascii_values.push_back(0);
	}
    }
    else {
	ascii_values.at(idx) += 1;
    }
}

void Hash::get_next() {
    char c{};
    
    while (true) {
	Hash::operator++();

	string hashstr = Hash::get();
	int strlen = hashstr.length();
	string str{};
	bool found_it = true;

	fstream fs(this->filename, fstream::in);
	while(strlen-- && fs >> noskipws >> c) {
	    str += string(1, c);
	}

	while (true) {
	    // if we have reached end of file
	    if (!(fs >> noskipws >> c))
		break;
	    
	    if (hashstr == str) {
		found_it = false;
		break;
	    }
	    
	    // step string forward 
	    str += string(1, c);
	    
	    str.erase(0, 1);
	}

	if (found_it) {
	    return;
	}
    }
}
