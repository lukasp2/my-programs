#include "finished.cc"

void Translator::get_all_strings(map<string, int>& strings, int series_size) {
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

// get all strings we would like to translate in the file
void Translator::get_strings_to_translate() {
    int string_size{ 2 };
    
    // find series of characters of size series_size
    while (true) {
	map<string, int> all_strings{};
	
	get_all_strings(all_strings, string_size);

	bool found_word_to_replace = false;
	
	// remove all strings from the map not worthy of an alias
	for (auto it = all_strings.begin(); it != all_strings.end(); ++it) {
	    if (expected_savings(it->first.length(), it->second, 1) > 0) {
		
		remove_obsolete_entries(it->first, it->second);
		
		str_ptrs[it->first] = new string{it->first};

		if (verbose) { cout << str_ptrs.size() << " candidate strings found  \r" << flush; }

		occurences[str_ptrs[it->first]] = it->second;

		found_word_to_replace = true;
	    }
	}
	
	if (!found_word_to_replace)
	    break;
	
	found_word_to_replace = false;
	
	++string_size;
    }

    if (verbose) { cout << endl; }
}

string* Translator::get_next_word_from_schedule() {
    auto max_lucra{ schedule.begin() };
    
    for (auto it{ schedule.begin() }; it != schedule.end(); ++it) {
	if ( lucrativity[*it] > lucrativity[*max_lucra] ) {
	    max_lucra = it;
	}
    }

    string* str_ptr{*max_lucra};

    schedule.erase(max_lucra);
    superstrings.erase(*max_lucra);
    substrings.erase(*max_lucra);
    
    return str_ptr;
}

// when a translation is made, substrings and superstrings needs to be updated. 
void Translator::update_substrs_n_sprstrs() {
    for (string* s : schedule) {
	if (substrings.find(s) != substrings.end()) {
	    // looping through substrings of string s
	    for (uint idx{}; idx < substrings[s].size(); ++idx) {
		if (!(is_substr(*substrings[s][idx], *s))) {
		    substrings[s].erase(substrings[s].begin() + idx);
		}
	    }
	}

	if (superstrings.find(s) != superstrings.end()) {
	    // looping through superstrings of string s
	    for (uint idx{}; idx < superstrings[s].size(); ++idx) {
		if (!(is_substr(*s, *superstrings[s][idx]))) {
		    superstrings[s].erase(superstrings[s].begin() + idx);
		}		
	    }
	}
    }
}

void Translator::replace_str_in_schedule(string* word, string const hash_val) {    
    string old_word = *word;
    *word = hash_val;
    
    // replace *word in the superstrings and update lucra
    for (string* superstring : superstrings[word]) {
	// 1. find *word in *superstring and replace with hashval
	replaceAll(*superstring, old_word, hash_val);
	
	// 2. change lucrativity of the *superstring
	lucrativity[superstring] = expected_savings(superstring->length(),
						    occurences[superstring],
						    hash_val.length());

	// 3. the superstring is changed, validate its' substrings.
	for (auto it{ substrings[superstring].begin() }; it != substrings[superstring].end(); ) {
	    string* substr = *it;

	    // if *substr is no longer a substring of *superstring, we need to:
	    // 1. remove *substr from substrings[superstring]
	    // 2. remove *superstring from superstrings[substring]
	    if (!is_substr(*substr, *superstring)) {
		for (auto it2 { superstrings[substr].begin() }; it2 != superstrings[substr].end(); ++it2) {
		    string* superstring2 = *it2;
		    
		    if (superstring2 == superstring) {
			superstrings[substr].erase(it2);
			break;
		    }
		}
		// remove substr from substrings[superstring]
		it = substrings[superstring].erase(it);
	    }
	    else ++it;
	}
    }
    
    for (string* substr : substrings[word]) {
	occurences[substr] -= occurences[word];
	    
	lucrativity[substr] = expected_savings(substr->length(),
					       occurences[substr],
					       hash_val.length());
    }

    substrings[word].clear();
}

// finds dependencies among the strings
void Translator::find_dependencies() {
    int ctr{ 1 };

    for (auto& p1 : str_ptrs) {
	if (verbose) { cout << "finding dependencies" << setw(7) << right << ctr++ << "/" << str_ptrs.size() << "\r" << flush; }
	
	for (auto& p2 : str_ptrs) {
	    if (is_substr(p1.first, p2.first)) {
		superstrings[str_ptrs[p1.first]].push_back(str_ptrs[p2.first]);
		substrings[str_ptrs[p2.first]].push_back(str_ptrs[p1.first]);
	    }
	}
    }
    
    if (verbose) { cout << endl; }
}

// create a translation map for each word that should be translated
void Translator::create_translations() {
    Hash hash{ filename };

    int ctr{ 1 };
    
    // translate the strings in schedule
    while (schedule.size() > 0) {
	
	if (verbose) { cout << "creating translations" << setw(7) << right << ctr++ << "/" << schedule.size() << "\r" << flush; }

	// pick most lucrative word from schedule.
	string* str_ptr = get_next_word_from_schedule();

 	if (lucrativity[str_ptr] <= 0)
	    break;

	hash.get_next();

	translations.push_back(make_pair(*str_ptr, hash.get()));

	auto start_time = std::chrono::high_resolution_clock::now();
	replace_str_in_schedule(str_ptr, hash.get());
	auto current_time = std::chrono::high_resolution_clock::now();

	if (false) {
	    // took 49
	    cout << "replacing str in schedule took "
		 << std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() << "ms" << endl;
	}

	// remove entry from occs[] and lucra[]
	// lucrativity.erase(*max_lucra);
	// occurences.erase(*max_lucra);
    }
    if (verbose) { cout << endl; }
}
