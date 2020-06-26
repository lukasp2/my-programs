#include "translator.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <utility>

#include <iostream>
#include <iomanip>

using namespace std;

////// ENCODING //////

void get_candidate_words(map<string, int>& char_series, int series_size, string const& filename);

int expected_savings(ulong str_length, int occurances, int hash_length);

void get_char_series(map<string, int>& char_series, string const& filename, int series_size);

struct comp {
    template<typename T>
    bool operator()(const T& a, const T& b) const {
	return expected_savings(a.first.length(), a.second, 1)
	    >= expected_savings(b.first.length(), b.second, 1);
    }
};

void remove_obsolete_entries(map<string, int>& char_series, auto const& it);
    
void create_translations(map<string, string>& translations,
			 set<pair<string, int>, comp> const& set,
			 string const& filename);

void write_file(string const& filename, map<string, string> const& translations);

template <typename T1, typename T2>
void print_map(map<T1, T2> const& m) {
    for (auto const& it : m)
	cout << setw(5) << left << it.second << " \"" << it.first << "\"" << endl;
}

/* 
 * the idea is:
 * 1. find strings in the file which are long enough, or occurs frequently 
 *    enough, that makes it worth replacing those strings with a shorter alias.
 * 2. rewrite the file with the translations.
 */
void Translator::encode(string const filename) {

    int series_size{ 2 };

    // 1. count the number of avaiable 1-character-hashes there are.
    // 2. when char_series reaches the same size as the no. of available hashes
    // of size 1, sort the map according to bytes that would be saved.

    // get all words of size series_size we would like to translate from file into char_series
    map<string, int> char_series{};
    get_candidate_words(char_series, series_size, filename);

    // the set will list the candidate words sorted based on how many bytes the
    // translation will save.
    set<pair<string, int>, comp> set(char_series.begin(), char_series.end());

    // stores candidate words and their translations 
    map<string, string> translations{};
    create_translations(translations, set, filename);

    //write_file(translations);
}

void get_candidate_words(map<string, int>& char_series,
			 int series_size,
			 string const& filename) {
    while (true) {
        // find series of characters of size series_size
	map<string, int> all_series{};
	get_char_series(all_series, filename, series_size);

	bool found_word_to_replace = false;
	
	// remove all strings from the map not worthy of an alias
	for (auto it = all_series.begin(); it != all_series.end(); ++it) {
	    if (expected_savings(it->first.length(), it->second, 1) > 0) {
		
		remove_obsolete_entries(char_series, it);
		    
		char_series.insert(make_pair(it->first, it->second));

		found_word_to_replace = true;
	    }
	}
	
	if (!found_word_to_replace)
	    break;
	
	found_word_to_replace = false;
	
	++series_size;
    }
}

// ex. say "th" has 5 occurences and is a candidate for getting an alias in the encoded file,
// but "the" was now discovered with atleast 4 occurences. Then the entry "th" is made obsolete
// by the entry "the".
void remove_obsolete_entries(map<string, int>& char_series, auto const& it) {
    string str = it->first;
    int occurences = it->second;
    
    string substr = str.substr(0, str.length() - 1);
    if (char_series.find(substr) != char_series.end()
	&& char_series[substr] - occurences < 2)
	char_series.erase(substr);

    substr = str.substr(1, str.length());
    if (char_series.find(substr) != char_series.end() && char_series[substr] - occurences < 2)
	char_series.erase(substr);
}

void get_char_series(map<string, int>& char_series,
				 string const& filename,
				 int series_size) {
    fstream fs(filename, fstream::in);
    char c;
    string str;

    // construct string
    while(series_size-- && fs >> noskipws >> c) {
	str += string(1, c);
    }

    while (true) {
	// if exists: add occurence, else make pair
	if (char_series.find(str) != char_series.end())
	    char_series[str]++;
	else 
	    char_series.insert(make_pair(str, 1));

        // if we have reached end of file
	if (!(fs >> noskipws >> c))
	    break;
	
	// step string forward 
	str += string(1, c);
	str.erase(0, 1);
    }
}

// returns the number of bytes saved expected by a translation to a hash
int expected_savings(ulong str_length, int occurances, int hash_length) {
    // current amount of bytes consumed by the string
    ulong without_alias = str_length * occurances;

    // bytes required for the map entry if alias is used
    ulong map_entry_bytes = hash_length + str_length;

    // amount of bytes consumed in the file if an alias is used
    int alias_in_file_bytes = hash_length * occurances;

    // total bytes consumed by an alias
    int with_alias = map_entry_bytes + alias_in_file_bytes;
        
    return without_alias - with_alias;
}

void create_translations(map<string, string>& translations,
			 set<pair<string, int>, comp> const& set,
			 string const& filename) {
    Hash hash{ filename };

    // maps debugging info (word to bytes saved in the translation)
    map<string, int> saved_bytes{};
    
    // find words which are worth translating to a smaller word (the hash).
    for (auto const &p: set) {
        // find a hash value
	hash.get_next();
	
	// if a translation would consume less bytes than not translating
	if (expected_savings(p.first.length(), p.second, hash.get().size()) > 0) {
	    saved_bytes.insert(make_pair(p.first, expected_savings(p.first.length(), p.second, hash.get().size())));
	    
	    translations.insert(make_pair(p.first, hash.get()));
	}
	// we will get to the else branch if the hash is too long for
	// a translation to save bytes.
	else {
	    break;
	}
    }

    int tot_saving{};
    cout << "translations [from word -> to hash]" << endl;
    for (auto& p : translations) {
	string s = "\"" + p.first + "\" -> " + p.second;
	cout << "\t" << setw(25) << left << s;
	cout << "(saving " << saved_bytes[p.first] << " bytes)" << endl;
	tot_saving += saved_bytes[p.first];
    }
    cout << "tot_svaving "<< tot_saving << endl;
}

void write_file(string const& filename, map<string, string> const& translations) {
    string s{};
    fstream fin(filename, fstream::in);

    while (fin >> s) {
	if (translations.find(s) != translations.end()) {
	    s = translations.at(s);
	}
	cout << s;
    }
}
// .. //

////// DECODING //////
void Translator::decode(std::string const filename) {
    string s = filename;
    s = "";
    cout << s << endl;
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
    fstream fs(this->filename, fstream::in);
    string str{};
    char c{};

    while (true) {
	Hash::operator++();

	string hashstr = Hash::get();
	int strlen = hashstr.length();
	bool found_it = true;
	
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

	if (found_it)
	    return;
    }
}
// .. //
