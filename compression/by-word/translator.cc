#include "translator.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <utility>

#include <iostream>
#include <iomanip>

using namespace std;

////// ENCODING //////
struct comp {
    template<typename T>
    bool operator()(const T& a, const T& b) const {
	    return a.first.length() * a.second > b.first.length() * b.second;
    }
};

map<string, int> read_file(std::string const filename);

map<string, string> create_translations(set<pair<string, int>, comp> set,
					map<string, int> unique_words);

void write_file(map<string, string> translations);

// the idea is ...
// 1. sort file by words which consume most bytes
// 2. find words which are worth translating to a smaller word.
// 3. create a map containing those translations
// 4. rewrite the file with the translations.
void Translator::encode(std::string const filename) {

    // stores each unique word and its no. of occurences in the file
    map<string, int> unique_words = read_file(filename);

    // the map is copied to a set and sorted based on how many total
    // bytes the words consume.
    set<pair<string, int>, comp> set(unique_words.begin(), unique_words.end());

    // stores candidate words and their translations
    map<string, string> translations = create_translations(set, unique_words);

    write_file(translations);
}

map<string, int> read_file(std::string const filename) {
    map<string, int> unique_words{};
	
    char c{};
    string s{};
    fstream fin(filename, fstream::in);

    while (fin >> noskipws >> c) {
	// make sure . and , is not included in the string isaplha()?
	if (!isspace(c)) {
	    s += c;
	}
	else {
	    string whitespace{1, c};
	    unique_words[s]++;
	    unique_words[whitespace]++;
	    s = string{};
	}
    }

    return unique_words;
}

map<string, string> create_translations(set<pair<string, int>, comp> set,
					map<string, int> unique_words) {
    map<string, string> translations{};
	
    Hash hash{};

    // maps debugging info (word to bytes saved in the translation)
    map<string, int> saved_bytes{};
    
    // find words which are worth translating to a smaller word (the hash).
    for (auto const &p: set) {
	
        // find a hash value that is not already used in the file
	while (unique_words.find(hash.get()) != unique_words.end()) ++hash;

	// print all words and the bytes consumed
	//cout << "{" << p.first << ", " << p.second << "} bytes: " << p.first.length() * p.second;
	
	// if a translation would consume less bytes than not translating
	if (p.first.length() * p.second
	    > hash.get().length() + p.first.length() + hash.get().length() * p.second) {

	    saved_bytes.insert(make_pair(p.first, p.first.length() * p.second - (1 + p.first.length() + 1 * p.second)));
	    
	    translations.insert(make_pair(p.first, hash.get()));
	    
	    ++hash;
	}
    }

    cout << "translations [from word -> to hash]" << endl;
    for (auto& p : translations) {
	string s = "\"" + p.first + "\" -> " + p.second;
	cout << "\t" << setw(25) << left << s;
	cout << "(saving " << saved_bytes[p.first] << " bytes)" << endl;
    }
    
    return translations;
}

void write_file(map<string, string> translations) {
    char c{};
    string s{};
    fstream fin(filename, fstream::in);

    while (fin >> s) {
	if (translations.find(s) != translations.end()) {
	    s = translations[s];
	}
	cout << s;
    }
}
// .. //

////// DECODING //////
void read_encoded_file();
    
void Translator::decode(std::string const filename) {
    read_encoded_file();
}

void read_encoded_file() {

}
// .. //

////// CLASS: HASH //////
// gets the hash
std::string Hash::get() {
    string s{};

    for (int i : ascii_values) {
	s += static_cast<char>(i);
    }
    
    return s;
}

Hash& Hash::operator++() {
    incr_pos(0);
    
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
// .. //
