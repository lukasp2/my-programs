#include "translator.h"
#include "hash.h"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <functional>
#include <utility>

#include <iostream>
#include <iomanip>

using namespace std;

#include "helpers.cc"

/* 
 * the idea is to find strings in the file which are long enough, or occurs frequently 
 * enough, that makes it worth replacing those strings with a shorter alias.
 */
void Translator::encode(string const filename) {
    this->filename = filename;
    
    get_strings_to_translate();

    find_dependencies();

    create_schedule();

    create_translations();

    // free memory here
    
    write_file();
}

void Translator::write_file() {
    // for each translation in "translations", replace all occurences of p.first in file
    // with p.second
    for (pair<string, string> const& p: translations) {
    
	fstream fs(filename, fstream::in);
	char c;
	string str;
	
	uint series_size = p.first.length();
	
	// construct string
	while(series_size-- && fs >> noskipws >> c) {
	    str += string(1, c);
	}

	while (true) {
	    if (str == p.first) {
		// replace str with p.second
	    }
	    
	    // if we have reached end of file
	    if (!(fs >> noskipws >> c))
		break;
	
	    // step string forward 
	    str += string(1, c);
	    str.erase(0, 1);
	}
    }
}

void Translator::decode(string const filename) {
    string s = filename; s = ""; cout << s;
}
