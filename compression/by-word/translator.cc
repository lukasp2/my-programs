#include "translator.h"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <functional>
#include <utility>
#include <list>

#include <iostream>
#include <iomanip>

#include "helpers.cc"

using namespace std;

void schedule_translations(map<string, int>& strings,
			   vector<pair<string*, int>>& schedule);

void create_translations(map<string, string>& translations,
			 list<pair<string, int>> const& list,
			 string const& filename);

/* 
 * the idea is to find strings in the file which are long enough, or occurs frequently 
 * enough, that makes it worth replacing those strings with a shorter alias.
 */
void Translator::encode(string const filename) {
    // get all strings we would like to translate into strings
    // map<string, no. occurences in the file>
    map<string, int> strings{};
    get_strings_to_translate(strings, filename);
    
    // create list containing all strings sorted based on in what order they should
    // be translated.
    vector<pair<string*, int>> schedule{};
    schedule_translations(strings, schedule);

    // create a translation map for each word that should be translated
    //map<string, string> translations{};
    //create_translations(translations, schedule, filename);
    
    // write to the file using the translation map
    //write_file(translations, filename);
}

// Decides in what order the translations occur. Sort strings based on lucrativity
void schedule_translations(map<string, int>& strings,
			   vector<pair<string*, int>>& schedule) {
    // strings should be map<string*, int>??
    
    // allocate memory on heap for all strings
    map<string, string*> s_map{};
    for (auto const& p : strings) {
	s_map[p.first] = new string{p.first};
    }

    for (auto const& p : strings) {
	schedule.push_back(
	    make_pair(s_map[p.first],
		      expected_savings(p.first.length(), p.second, 1)
		));
    }

    // find dependecies
    map<string*, vector<string*>> str_to_substr{}, substr_to_str{};
    for (auto& p1 : strings) {
	for (auto& p2 : strings) {
	    if (is_substr(p1.first, p2.first))
		substr_to_str[s_map[p1.first]].push_back(s_map[p2.first]);
         	// replace below with str_to_substr[] here instead???
	    
	    else if (is_substr(p2.first, p1.first))
		str_to_substr[s_map[p1.first]].push_back(s_map[p2.first]);
	}
    }

    print_schedule(schedule);
        
    // .... //
    for (uint i{}; i < schedule.size(); ++i) {

	// sorting the schedule based on lucrativeness
	sort(schedule.begin() + i, schedule.end(),
	     [&strings] (auto const& p1, auto const& p2) {
		 return p1.second > p2.second;
	     });

	print_schedule(schedule);

	
	string* word = schedule[i].first;
	
	// replace *word in the superstrings
	for (string* str : substr_to_str[word]) {
	    // 1. find *word in *str and replace with "$"
	    replaceAll(*str, *word, "$");
	    
	    // 2. change lucrativity of *str in schedule
	    schedule[str].second
		= expected_savings(str->length(), strings[*str].second, 1);
	}

	// change occurences of substrings
	for (string* substr : str_to_substr[word]) {
	    strings[*substr] -= strings[*word];
	    
	    // re-calibrate lucrativeness of the substrings
	    schedule[substring].second
		= expected_savings(substr->length(), strings[*substr].second, 1);
	}

	// translate
	*word = "$";
	
	break;
    }
    // .... //
    
    // print schedule
    print_schedule(schedule);

    print_deps(str_to_substr);
    print_deps(substr_to_str);

    // destruct s_map
    //for (auto& p : s_map)
    //	delete p.second;
}

void create_translations(map<string, string>& translations,
			 list<pair<string, int>> const& schedule,
			 string const& filename) {
    // This changes " the " to "$"
    // *(schedule[0].first) = "$";
    
    Hash hash{ filename };

    bool print_translations = false;
    if (print_translations) {
	cout << setw(8) << left << "occs." << setw(35) << left << "string"
	     << setw(15) << left << "bytes consump."
	     << setw(15) << left << "replacement"
	     << setw(15) << left << "bytes consump."
	     << setw(6) << left << "diff" << endl;
    }
    int tot_savings{};
    
    // find words which are worth translating to a smaller word (the hash).
    for (auto const &p: schedule) {
        // find a hash value
	hash.get_next();
	
	// if a translation would consume more bytes than not translating
	if (expected_savings(p.first.length(), p.second, hash.get().size()) <= 0)
	    break;

	translations.insert(make_pair(p.first, hash.get()));

	if (print_translations) {
	    cout << setw(8) << left << p.second
		 << setw(35) << left << "'" + p.first + "'"
		 << setw(15) << left << p.second *  p.first.length()
		 << setw(15) << left << "'" + hash.get() + "'"
		 << setw(15) << left << p.second *  hash.get().length() + hash.get().length() + p.first.length()
		 << setw(6) << left << expected_savings(p.first.length(), p.second, hash.get().length())
		 << endl;
	    tot_savings += expected_savings(p.first.length(), p.second, hash.get().length());
	}
    }

    if (print_translations) {
	cout << "\ntotal savings: " << tot_savings << " bytes" << endl;
    }
}
