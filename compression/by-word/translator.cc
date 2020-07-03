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

void schedule_translations(map<string, int> const& strings,
			   vector<pair<string*, vector<string*>>>& schedule);

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
    // be translated.
    vector<pair<string*, vector<string*>>> schedule{}; // schedule + deps
    schedule_translations(strings, schedule);

    // create a translation map for each word that should be translated
    //map<string, string> translations{};
    //create_translations(translations, schedule, dependecies, filename);
    
    // write to the file using the translation map
    //write_file(translations, filename);
}

// Decides in what order the translations occur. Sort strings based on lucrativity
void schedule_translations(map<string, int> const& strings,
			   vector<pair<string*, vector<string*>>>& schedule) {

    map<string, string*> s_map{};
    for (auto const& p : strings) {
	s_map[p.first] = new string{p.first};
    }
    
    // find dependecies and add them to vector "dependencies"
    for (auto& p1 : strings) {
	vector<string*> v{};
	
	for (auto& p2 : strings)
	    if (is_substr(p1.first, p2.first))
		v.push_back(s_map[p2.first]);

	schedule.push_back(make_pair(s_map[p1.first], std::move(v)));
    }

    auto schedule_comp =
	[&strings](auto const& p1, auto const& p2) {
	    return expected_savings(p2.first->length(), strings.at(*p2.first), 1)
		< expected_savings(p1.first->length(), strings.at(*p1.first), 1);
	};
    
    // sorting the schedule based on lucrativeness
    sort(schedule.begin(), schedule.end(), schedule_comp);
    
    // go through schedule, for each depends. change its string and lucrativeness

    // .... ///
    // 1. replace " the " in the strings in .second

    for (string* s : schedule.front().second) {
	auto it = find(schedule.front().second.begin(),
		       schedule.front().second.end(),
		       schedule.front().first);
    }
	
    *p.first = "$";

    // 2. change lucrativity value of all strings which is a substring of " the "
    // .... ///
    
    // print schedule
    print_schedule(schedule, strings);

    // destruct s_map
    for (auto& p : s_map)
	delete p.second;
    
    // This changes " the " to "$"
    // *(schedule[0].first) = "$";
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
