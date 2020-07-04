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

void get_strings(map<string, int>& strings,
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
	if (strings.find(str) != strings.end())
	    strings[str]++;
	else 
	    strings.insert(make_pair(str, 1));

        // if we have reached end of file
	if (!(fs >> noskipws >> c))
	    break;
	
	// step string forward 
	str += string(1, c);
	str.erase(0, 1);
    }
}

// returns the number of bytes saved expected by a translation to a hash
int expected_savings(ulong const str_length, int const occurances, int const hash_length) {
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

// ex. say "th" has 5 occurences and is a candidate for getting an alias in the
// encoded file, but "the" was now discovered with atleast 4 occurences. Then
// the entry "th" is made obsolete by the entry "the".
void remove_obsolete_entries(map<string, int>& strings, auto const& it) {
    string str = it->first;
    int occurences = it->second;
    
    string substr = str.substr(0, str.length() - 1);
    if (strings.find(substr) != strings.end()
	&& strings[substr] - occurences < 2) {
	strings.erase(substr);
    }
	
    substr = str.substr(1, str.length());
    if (strings.find(substr) != strings.end()
	&& strings[substr] - occurences < 2) {
	strings.erase(substr);
    }
}

//void get_candidate_words(map<string, int>& strings, string const& filename) {
void get_strings_to_translate(map<string, int>& strings, string const& filename) {
    int string_size{ 2 };
    
    while (true) {
        // find series of characters of size series_size
	map<string, int> all_strings{};
	get_strings(all_strings, filename, string_size);

	bool found_word_to_replace = false;
	
	// remove all strings from the map not worthy of an alias
	for (auto it = all_strings.begin(); it != all_strings.end(); ++it) {
	    if (expected_savings(it->first.length(), it->second, 1) > 0) {
		
		remove_obsolete_entries(strings, it);

		strings.insert(make_pair(it->first, it->second));

		found_word_to_replace = true;
	    }
	}
	
	if (!found_word_to_replace)
	    break;
	
	found_word_to_replace = false;
	
	++string_size;
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

// print schedule and dependencies
void print_schedule(vector<pair<string*, int>> const& schedule) {
    cout << setw(35) << left << "string" << setw(15) << left << "savings" << endl;
    
    for (auto const& p : schedule) {
	cout << setw(35) << left << "'" + *p.first + "'"
	     << setw(15) << left << p.second << endl;
    }
}

void print_deps(map<string*, vector<string*>> const& deps) {
    cout << endl << endl << setw(35) << left << "string"
	 << setw(15) << left << "deps" << endl;

    for (auto const& p : deps) {
	cout << setw(35) << left << "'" + *p.first + "'";

	cout << "[";
	for (string* s : p.second) {
	    cout << *s + ", ";
	}
	cout << "]" << endl;
    }
}

// in string str: replace substring "from" with substring "to"
void replaceAll(string& str, string const& from_str, string const& to_str) {
    if(from_str.empty())
        return;
    
    auto found = str.find(from_str, 0);
    while(found != std::string::npos) {
        str.replace(found, found + from_str.length(), to_str);
	
	// in case 'to' contains 'from', like replacing 'x' with 'yx'
        found += to_str.length();

	found = str.find(from_str, found);
    }
}

////// DECODING //////
void Translator::decode(string const filename) {
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

