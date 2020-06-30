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

// Decides in what order the translations occur. E.x. if we have two words to translate:
// 'he' and 'the', the translation of 'he' is done first because 'he' is a substring to 'the'
void schedule_translations(list<pair<string, int>>& schedule,
			   map<string, int>& unsorted_map,
			   map<string, vector<string>>& dependencies) {
    // 1. schedule all root words first
    // 2. sort root words based on lucrativity

    // a list of lists:
    // list 1 contains root words
    // list N contains the superwords for list N - 1 and composition words for list N + 1

    // divide the strings in levels
    vector< vector<pair<string, int>>* > vector_ptrs{};
    while(unsorted_map.size() > 0) {
	auto vector_ptr = new vector<pair<string, int>>{};
	vector_ptrs.push_back(vector_ptr);

	// we are looking for root words. root words needs to be translated first.
	for (auto const& pair1 : unsorted_map) {
	    string word_to_schedule{ pair1.first };
	    bool root_word{ true };

	    // is there any word which is a substring to word_to_schedule?
	    // in that case word_to_schedule is not a root word and should not be
	    // scheduled right now.	    
	    for (auto const& pair2 : unsorted_map) {
		if (is_substr(pair2.first, word_to_schedule)) {
		    dependencies[word_to_schedule].push_back(pair2.first);
		    root_word = false;
		}
	    }

	    // we have found a root word. it needs to be removed for the next iteration.
	    if (root_word) {
		vector_ptr->push_back(pair1);
	    }
	}

	for (auto const& p : *vector_ptr) {
	    unsorted_map.erase(p.first);
	}
    }

    // sorting the vectors based on lucrativeness
    // ??? DOES THIS EVEN WORK? How do we know how lucrative it is?
    for (auto vector_ptr : vector_ptrs) {
	sort(vector_ptr->begin(), vector_ptr->end(),
	     [](pair<string, int> const& p1, pair<string, int> const& p2) {
		 return expected_savings(p1.first.length(), p1.second, 1)
		     >= expected_savings(p2.first.length(), p2.second, 1);
	     });
    }

    // sort and parse the dependencies
    for (auto& p : dependencies) {
	sort(p.second.begin(), p.second.end(),
	     [](string const& s1, string const& s2) {
		 return s1 > s2;
	     });
	
	p.second.erase(unique(p.second.begin(), p.second.end()), p.second.end());
    }

    // construct the return value
    for (auto const& vector_ptr : vector_ptrs) {
	for (auto const& pair : *vector_ptr) {
	    schedule.push_back(pair);
	}
    }

    // print schedule and dependencies
    cout << setw(25) << left << "string"
	 << setw(15) << left << "savings"
	 << setw(20) << left << "dependencies" << endl;
    
    for (auto & p : schedule) {
	cout << setw(25) << left << "'" + p.first + "'"
	     << setw(15) << left << expected_savings(p.first.length(), p.second, 1)
	     << "[";
	
	for (string& s : dependencies[p.first])
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
