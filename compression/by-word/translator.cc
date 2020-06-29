#include "translator.h"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <functional>
#include <utility>
#include <list>

#include <iostream>
#include <iomanip>

using namespace std;

////// ENCODING //////

void get_candidate_words(map<string, int>& char_series, string const& filename);

int expected_savings(ulong str_length, int occurances, int hash_length);

void get_char_series(map<string, int>& char_series, string const& filename, int series_size);

//void schedule_translations(list<pair<string, int>>& list, map<string, int> const& unsorted_map);
void schedule_translations(list<pair<string, int>>& list, map<string, int>& unsorted_map);

void remove_obsolete_entries(map<string, int>& char_series, auto const& it);
    
void create_translations(map<string, string>& translations,
			 list<pair<string, int>> const& list,
			 string const& filename);

void write_file(string const& filename, map<string, string> const& translations);

/* 
 * the idea is to find strings in the file which are long enough, or occurs frequently 
 * enough, that makes it worth replacing those strings with a shorter alias.
 */
void Translator::encode(string const filename) {
    // get all strings we would like to translate into char_series
    // map<string, no. occurences in the file>
    map<string, int> strings{};
    get_candidate_words(strings, filename);
    // rename: get words to translate
    
    // create list containing all strings sorted based on in what order they should
    // be translated. Eg. the translation 'he ' -> '$' happens before ' the ' -> '@'
    // so that instead they become 'he ' -> '$' and ' t$' -> '@'.
    list<pair<string, int>> schedule;
    schedule_translations(schedule, strings);
    
    // use the one-length-aliases in the most lucrative translations
    // >> schedule all the root words first
    // >> sort root words based on lucrativity

    // create a translation map for each word that should be translated
    map<string, string> translations{};
    //create_translations(translations, schedule, filename);
    
    // write to the file using the translation map
    //write_file(translations, filename);
}

// returns true iif substr is a part of str but not equal to str
bool is_substr(string const& substr, string const& str) {
    return str.find(substr) != std::string::npos
	&& substr != str;
}

void schedule_translations(list<pair<string, int>>& schedule, map<string, int>& unsorted_map) {
    // 2. mark all root words
    // 3. schedule all root words first
    // 4. sort root words based on lucrativity

    // a list of lists:
    // list 1 contains root words
    // list N contains the superwords for list N - 1 and composition words for list N + 1
    for (auto i : schedule) cout << i.first << " ";

    list< list<pair<string, int>>* > llist{};

    while(unsorted_map.size() > 0) {

	auto list_ptr = new list<pair<string, int>>{};
	llist.push_back(list_ptr);

	for (auto const& pair1 : unsorted_map) {
	    string word_to_schedule{ pair1.first };
	    bool root_word{ true };

	    // if word_to_schedule is a root word, add to list
	    for (auto const& pair2 : unsorted_map) {
		// p2 is a substring of word_to_schedule (=> word_to_schedule is not root)
		if (is_substr(pair2.first, word_to_schedule)) {
		    root_word = false;
		    break;
		}
	    }

	    if (root_word) {
		list_ptr->push_back(pair1);
		unsorted_map.erase(pair1.first);
	    }
	}
    }

    int prio{ 1 };
    for (auto list_ptr : llist) {
	cout << "words prio" << prio++ << endl;
	
	for (auto pair_ : *list_ptr) {
	    cout << "'" + pair_.first + "'" << endl;
	}
	cout << endl;
    }
}

void get_candidate_words(map<string, int>& char_series, string const& filename) {
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
			 list<pair<string, int>> const& list,
			 string const& filename) {
    Hash hash{ filename };

    cout << setw(8) << left << "occs." << setw(35) << left << "string"
	 << setw(15) << left << "bytes consump."
	 << setw(15) << left << "replacement"
	 << setw(15) << left << "bytes consump."
	 << setw(6) << left << "diff" << endl;
    int tot_savings{};
    
    // find words which are worth translating to a smaller word (the hash).
    for (auto const &p: list) {
        // find a hash value
	hash.get_next();

	// if a translation would consume more bytes than not translating
	if (expected_savings(p.first.length(), p.second, hash.get().size()) <= 0)
	    break;

	translations.insert(make_pair(p.first, hash.get()));

	cout << setw(8) << left << p.second
	     << setw(35) << left << "'" + p.first + "'"
	     << setw(15) << left << p.second *  p.first.length()
	     << setw(15) << left << "'" + hash.get() + "'"
	     << setw(15) << left << p.second *  hash.get().length() + hash.get().length() + p.first.length()
	     << setw(6) << left << expected_savings(p.first.length(), p.second, hash.get().length())
	     << endl;
	tot_savings += expected_savings(p.first.length(), p.second, hash.get().length());
    }

    cout << "\ntotal savings: " << tot_savings << " bytes" << endl;
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
