#include <map>
#include <string>

#include <algorithm>
#include <iterator>
#include <fstream>
#include <functional>
#include <utility>
#include <list>

#include <iostream>
#include <iomanip>

using namespace std;

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

//void get_candidate_words(map<string, int>& char_series, string const& filename) {
void get_strings_to_translate(map<string, int>& char_series, string const& filename) {
    int series_size{2};
    
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

// returns true iif substr is a part of str but not equal to str
bool is_substr(string const& substr, string const& str) {
    return str.find(substr) != std::string::npos
	&& substr != str;
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

// .. //

