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

void schedule_translations(list<pair<string, int>>& list,
			   map<string, int>& unsorted_map,
			   map<string, vector<string>>& dependecies);


void create_translations(map<string, string>& translations,
			 list<pair<string, int>> const& list,
			 map<string, vector<string>> const& dependecies,
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
    // be translated. Eg. the translation 'he ' -> '$' happens before ' the ' -> '@'
    // so that instead they become 'he ' -> '$' and ' t$' -> '@'.
    list<pair<string, int>> schedule;
    map<string, vector<string>> dependecies{};
    schedule_translations(schedule, strings, dependecies);

    // create a translation map for each word that should be translated
    map<string, string> translations{};
    create_translations(translations, schedule, dependecies, filename);
    
    // write to the file using the translation map
    //write_file(translations, filename);
}

// Decides in what order the translations occur. Sort strings based on lucrativity
void schedule_translations(list<pair<string, int>>& schedule,
			   map<string, int>& unsorted_map,
			   map<string, vector<string>>& dependencies) {

    vector<pair<string, int>*> v{};
    
    // add dependecies
    for (auto& pair1 : unsorted_map) {
	// pair1.first = the word to schedule
	for (auto& pair2 : unsorted_map) {
	    if (is_substr(pair2.first, pair1.first)) {
		//dependencies[pair1.first].push_back(pair2.first); old
		dependencies[pair2.first].push_back(pair1.first);
	    }
	}

	pair<string, int>* xy = new pair<string, int>;
	xy = &pair1;
	v.push_back(xy);
    }

    // sorting the vector based on lucrativeness
    sort(v.begin(), v.end(),
	 [](pair<string, int>* const p1, pair<string, int>* const p2) {
	     return expected_savings(p1->first.length(), p1->second, 1)
		 >= expected_savings(p2->first.length(), p2->second, 1);
	 });

    // go through v, for each depends. change its string and lucrativeness
    // cannot convert
    // std::pair<const std::__cxx11::basic_string<char>, int>*’ to
    // std::pair<std::__cxx11::basic_string<char>, int>*’

    // construct the return value
    for (pair<string, int>* const pair_ptr : v) {
	schedule.push_back(*pair_ptr);
    }
    
    // print schedule and dependencies
    cout << setw(25) << left << "string"
	 << setw(15) << left << "savings"
	 << setw(20) << left << "dependencies" << endl;
    
    for (pair<string, int>* const p : v) {
	cout << setw(25) << left << "'" + p->first + "'"
	     << setw(15) << left << expected_savings(p->first.length(), unsorted_map[p->first], 1)
	     << "[";
	
	for (string const& s : dependencies[p->first])
	    cout << "'" << s << "', ";
	
	cout << "]" << endl;
    }
}
void create_translations(map<string, string>& translations,
			 list<pair<string, int>> const& schedule,
			 map<string, vector<string>> const& dependecies,
			 string const& filename) {
    for (auto p : dependecies) {}
    
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
	}
	tot_savings += expected_savings(p.first.length(), p.second, hash.get().length());
    }

    if (print_translations) {
	cout << "\ntotal savings: " << tot_savings << " bytes" << endl;
    }
}
