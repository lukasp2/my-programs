#include "translator.h"
#include "hash.h"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <functional>
#include <utility>

#include <iostream>
#include <iomanip>

using namespace std;

#include "helpers.cc"

/* 
 * the idea is to find strings in the file which are long enough, or occurs frequently 
 * enough, that makes it worth replacing those strings with a shorter alias.
 */
void Translator::encode() {
    get_strings_to_translate();

    find_dependencies();

    create_schedule();

    create_translations();

    exit(1);
    //write_file();
}

void Translator::replace_str_in_schedule(int schedule_pos, string const hash_val) {    
    string* word = schedule[schedule_pos];
    string old_word = *word;
    *word = hash_val;
    
    // replace *word in the superstrings and update lucra
    for (string* str : substr_to_str[word]) {
	// 1. find *word in *str and replace with hashval
	replaceAll(*str, old_word, hash_val);
	
	// 2. change lucrativity of *str
	lucrativity[str] = expected_savings(str->length(), occurences[str], 1);
    }
    
    for (string* substr : str_to_substr[word]) {
	occurences[substr] -= occurences[word];
	    
	lucrativity[substr]
	    = expected_savings(substr->length(), occurences[substr], 1);
    }
    
    // as we have now changed a lot, update substrings and superstrings
    update_substrs_n_sprstrs();
}

// remove str's from schedule with lucra <= 0
void Translator::refactor_schedule() {
    auto it = remove_if(schedule.begin(), schedule.end(),
			     [&lucrativity = lucrativity] (string* const s) {
				 return lucrativity[s] <= 0;
			     });

    schedule.erase(it, schedule.end());

    // sorting the schedule based on lucrativeness
    sort(schedule.begin(), schedule.end(),
	 [&lucrativity = lucrativity] (string* const s1, string* const s2) {
	     return lucrativity[s1] > lucrativity[s2];
	 });
}

// create a translation map for each word that should be translated
void Translator::create_translations() {
    Hash hash{ filename };

    refactor_schedule();
	
    // translate the strings in schedule
    for (uint idx{}; idx < schedule.size(); ++idx) {
	hash.get_next();
	
	translations.push_back(make_pair(*schedule[idx], hash.get()));

	replace_str_in_schedule(idx, hash.get());

	refactor_schedule();

	print_translations();	
    }

    //print_schedule();

    //print_translations();
}
