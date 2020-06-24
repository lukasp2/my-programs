#include <iostream>
#include <algorithm>
#include <fstream>
#include <queue>
#include "compressor.h"

using namespace std;

Compressor::~Compressor() {
    for (auto& p : char_map) delete p.second;
}

void Compressor::compress(string const filename) {
    ifstream file(filename);

    char_it it(file), end;

    for ( ; it != end; ++it) {
	for (auto & p: char_positions) {
	    if (p.first != *it)
		p.second++;
	}

	if (char_map.find(*it) == char_map.end()) {
	    char_map[*it] = new vector<int>{};
	    insertion_order.push_back(*it);
	}
	
	char_map[*it]->push_back(char_positions[*it]);
	char_positions[*it] = 0;
    }
}

void Compressor::decode() {
    while (insertion_order.size() > 0) {

	for (long unsigned int k{}; k < insertion_order.size(); ++k) {
	    int& val = (*char_map[insertion_order[k]])[0];

	    if (val < 0) break;
	    
	    --val;
	}

	for (long unsigned int k{}; k < insertion_order.size(); ++k) {
	    char c = insertion_order[k];
	    vector<int>* vec = char_map[c];
	    int& val = (*vec)[0];

	    if (val == -1) {
		cout << c;
	   
		vec->erase(vec->begin());
	    
		if (vec->size() == 0) {
		    insertion_order.erase(insertion_order.begin() + k);
		}
	    
		k = -1;

		break;
	    }
	}
    }
}

void Compressor::print(int i) const {
    int k{};
    cout << endl;
    for (char c : insertion_order) {
	cout << c << " ";

       	for (int const& i : *char_map.at(c))
	    cout << i << " ";

	if (k++ > i) break;

	cout << endl;
    }
    cout << endl;
}

/*
compile: g++ -g main.cc compression.cc
gdb ./a.out
break [function name]
file ./a.out
run ./a.out

*/
