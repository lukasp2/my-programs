#include "finished.cc"

// checks if str is worthy of getting translated
bool Translator::worthy_candidate(string const& str) {
    string substr = str.substr(0, str.length() - 1);
    if (str_ptrs.find(substr) != str_ptrs.end()) {
	//&& occurences[str_ptrs[substr]] - occurences[str_ptrs[str]] >= 2) {
	return true;
    }
    
    substr = str.substr(1, str.length());
    if (str_ptrs.find(substr) != str_ptrs.end()) {
	//&& occurences[str_ptrs[substr]] - occurences[str_ptrs[str]] >= 2) {
	return true;
    }

    return false;
}

bool Translator::find_strings(int str_size, bool first_it) {
    fstream fs(filename, fstream::in);
    char c{};
    string str{};
    bool found{ false };
    
    // construct string
    while(str_size-- && fs >> noskipws >> c) {
	str += string(1, c);
    }

    while (true) {
	if (str_ptrs.find(str) != str_ptrs.end()) {
	    occurences[str_ptrs[str]]++;
	}
	else if (worthy_candidate(str) || first_it) {
	    if (verbose) { cout << str_ptrs.size() << " candidate strings found  \r" << flush; }
	    str_ptrs[str] = new string{ str };
            // add dependencies?
	    occurences[str_ptrs[str]] = 1;
	    found = true;
	}

        // if we have reached end of file
	if (!(fs >> noskipws >> c))
	    break;
	
	// step string forward 
	str += std::string(1, c);
	str.erase(0, 1);
    }

    // with occs in concideration, should we delete the substrings from str_ptrs?
    // store in vector, sort vector by alphanum, delete unworthy substrings.

    return found;
}

void Translator::get_strings_to_translate() {
    int string_size{ 2 };
    bool first_it{ true };
    
    // find series of characters of size string_size
    while (true) {
	bool found = find_strings(string_size, first_it);

	// if no new strings to translate were found
	if (!found)
	    break;
	
	first_it = false;
	
	++string_size;
    }
    
    if (verbose) { cout << endl; }
}

/*
// only fetch strings that is a substr of strs we already have
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
*/

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

    for (auto it1{ schedule.begin() }; it1 != schedule.end(); ++it1) {
	if (verbose) { cout << "finding dependencies" << setw(7) << right << ctr++ << "/" << str_ptrs.size() << "\r" << flush; }
	
	string* s1{ *it1 };
	for (auto it2{ it1 }; it2 != schedule.end() ; ++it2) {
	    string* s2{ *it2 };
	    if (is_substr(*s1, *s2)) {
		superstrings[s1].push_back(s2);
		substrings[s2].push_back(s1);
	    }
	    else if (is_substr(*s2, *s1)) {
		superstrings[s2].push_back(s1);
		substrings[s1].push_back(s2);
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

	replace_str_in_schedule(str_ptr, hash.get());
    }

    if (verbose) { cout << endl; }
}
