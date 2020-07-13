#include "translator.h"
#include "hash.h"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <functional>
#include <utility>

#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;

#include "helpers.cc"

void Translator::encode(string const filename) {
    this->filename = filename;

    get_strings_to_translate();

    calculate_lucrativeness();
    
    find_dependencies();

    create_schedule();

    create_translations();

    write_file();
}

void Translator::decode(string const filename) {
    string s = filename; s = ""; cout << s;
}
