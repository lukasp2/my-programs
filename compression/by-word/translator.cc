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

    cout << "finding strings to translate ... " << endl;
    get_strings_to_translate();
    cout << "found " << str_ptrs.size() << " candidate strings" << endl;
    
    cout << "finding dependencies ... " << endl;
    find_dependencies();

    cout << "creating schedule ... " << endl;
    create_schedule();

    cout << "creating translations ... " << endl;
    create_translations();
    cout << "created " << translations.size() << " translations" << endl;

    // free memory here?
    
    cout << "writing to file ... " << endl;
    write_file();
}

void Translator::write_file() {
    fstream ifs(filename, fstream::in);
    fstream ofs("out.txt", fstream::out);
    string line{};

    // for each translation in "translations", replace all occurences of p.first in file
    // with p.second
    while (getline(ifs, line)) {
	for (pair<string, string> const& p: translations) {
	    replaceAll(line, p.first, p.second);
	}
	ofs << line << endl;
    }
}

void Translator::decode(string const filename) {
    string s = filename; s = ""; cout << s;
}
