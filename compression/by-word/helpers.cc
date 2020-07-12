#include <string>

// returns true iif substr is a part of str but not equal to str
bool is_substr(string const& substr, string const& str) {
    return str.find(substr) != string::npos && substr != str;
}

// in string str: replace substring "from" with substring "to"
void replaceAll(string& str, string const& from_str, string const& to_str) {
    if(from_str.empty())
        return;

    auto substr_pos = str.find(from_str, 0);
    while(substr_pos != std::string::npos) {
        str.replace(substr_pos, from_str.length(), to_str);
	
	// in case 'to' contains 'from', like replacing 'x' with 'yx'
        substr_pos += to_str.length();

	substr_pos = str.find(from_str, substr_pos);
    }
}

// returns the number of bytes saved expected by a translation to a hash
int expected_savings(uint const str_length, int const occurances, int const hash_length) {
    // current amount of bytes consumed by the string
    uint without_alias = str_length * occurances;

    // bytes required for the map entry if alias is used
    uint map_entry_bytes = hash_length + str_length;

    // amount of bytes consumed in the file if an alias is used
    int alias_in_file_bytes = hash_length * occurances;

    // total bytes consumed by an alias
    int with_alias = map_entry_bytes + alias_in_file_bytes;
        
    return without_alias - with_alias;
}

Translator::~Translator() {
    for (auto& p : str_ptrs)
    	delete p.second;
}

// create a schedule which represents a queue for the translations
void Translator::create_schedule() {
    for (auto const& p : str_ptrs) {
	schedule.push_back(str_ptrs[p.first]);
    }
}

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

// finds dependencies among the strings
void Translator::find_dependencies() {
    for (auto& p1 : str_ptrs) {
	for (auto& p2 : str_ptrs) {
	    if (is_substr(p1.first, p2.first)) {
		substr_to_str[str_ptrs[p1.first]].push_back(str_ptrs[p2.first]);
		str_to_substr[str_ptrs[p2.first]].push_back(str_ptrs[p1.first]);
	    }
	}
    }
}

// ex. say "th" has 5 occurences and is a candidate for getting an alias in the
// encoded file, but "the" was now discovered with atleast 4 occurences. Then
// the entry "th" is made obsolete by the entry "the".
void Translator::remove_obsolete_entries(string const& str, int const occs) {
    string substr{};
    
    substr = str.substr(0, str.length() - 1);
    if (str_ptrs.find(substr) != str_ptrs.end()
	&& occurences[str_ptrs[substr]] - occs < 2) {

	occurences.erase(str_ptrs[substr]);
	str_ptrs.erase(substr);
    }
	
    substr = str.substr(1, str.length());
    if (str_ptrs.find(substr) != str_ptrs.end()
	&& occurences[str_ptrs[substr]] - occs < 2) {
	
	occurences.erase(str_ptrs[substr]);
	str_ptrs.erase(substr);
    }
}

// get all strings we would like to translate in the file
void Translator::get_strings_to_translate() {
    int string_size{ 3 };
    
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
		
		occurences[str_ptrs[it->first]] = it->second;

		found_word_to_replace = true;
	    }
	}
	
	if (!found_word_to_replace)
	    break;
	
	found_word_to_replace = false;
	
	++string_size;
    }

    // fill lucrativeness map
    for (auto const& it : str_ptrs) {
	lucrativity[it.second] = expected_savings(it.first.length(), occurences[it.second], 1);
    }
}

// when a translation is made, str_to_substr and substr_to_str needs to be updated. 
void Translator::update_substrs_n_sprstrs() {
    for (string* s : schedule) {
	if (str_to_substr.find(s) != str_to_substr.end()) {
	    // looping through substrings of string s
	    for (uint idx{}; idx < str_to_substr[s].size(); ++idx) {
		if (!(is_substr(*str_to_substr[s][idx], *s))) {
		    str_to_substr[s].erase(str_to_substr[s].begin() + idx);
		}
	    }
	}

	if (substr_to_str.find(s) != substr_to_str.end()) {
	    // looping through superstrings of string s
	    for (uint idx{}; idx < substr_to_str[s].size(); ++idx) {
		if (!(is_substr(*s, *substr_to_str[s][idx]))) {
		    substr_to_str[s].erase(substr_to_str[s].begin() + idx);
		}		
	    }
	}
    }
}

#include <chrono>

// remove str's from schedule with lucra <= 0
void Translator::refactor_schedule() {
    // erasing all with lucra <= 0
    schedule.erase(remove_if(schedule.begin(), schedule.end(),
			     [&lucrativity = lucrativity] (string* const s) {
				 return lucrativity[s] <= 0;
			     }), schedule.end());
    
    // sorting the schedule based on lucrativeness
    sort(schedule.begin(), schedule.end(),
	 [&lucrativity = lucrativity] (string* const s1, string* const s2) {
	     return lucrativity[s1] > lucrativity[s2];
	 });
}

void Translator::replace_str_in_schedule(string* word, string const hash_val) {    
    string old_word = *word;
    *word = hash_val;
    
    // replace *word in the superstrings and update lucra
    for (string* str : substr_to_str[word]) {
	// 1. find *word in *str and replace with hashval
	replaceAll(*str, old_word, hash_val);
	
	// 2. change lucrativity of *str
	lucrativity[str] = expected_savings(str->length(), occurences[str], hash_val.length());
    }
    
    for (string* substr : str_to_substr[word]) {
	occurences[substr] -= occurences[word];
	    
	lucrativity[substr]
	    = expected_savings(substr->length(), occurences[substr], hash_val.length());
    }
    
    // as we have now changed a lot, update substrings and superstrings
    update_substrs_n_sprstrs();
}

// create a translation map for each word that should be translated
void Translator::create_translations() {
    Hash hash{ filename };

    refactor_schedule();
    
    // translate the strings in schedule
    for (uint idx{}; idx < schedule.size(); ++idx) {
	cout << "finding hash ... " << endl; // takes 9 ms
	//auto start_time = std::chrono::high_resolution_clock::now();
	hash.get_next();
	//auto current_time = std::chrono::high_resolution_clock::now();
	//cout << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() << "ms" << endl;
	
	translations.push_back(make_pair(*schedule[idx], hash.get()));

	cout << "replaceing str in schedule ..." << endl; // takes 47 ms
	replace_str_in_schedule(schedule[idx], hash.get());
	
	cout << "refactoring schedule ..." << endl; // takes 59 ms
	refactor_schedule();
    }
}

// additionals: prints
void Translator::print_file() {
    std::ifstream f(filename);

    if (f.is_open())
        std::cout << f.rdbuf();
}


void Translator::print_schedule() {
    cout << endl;
    cout << setw(35) << left << "string"
	 << setw(15) << left << "occs."
	 << setw(15) << left << "savings" << endl;
    
    for (string* const s : schedule) {
	cout << setw(35) << left << "'" + *s + "'"
	     << setw(15) << left << occurences.at(s)
	     << setw(15) << left << lucrativity.at(s)
	     << endl;
    }
}

void Translator::print_translations() {
    cout << "TRANSLATIONS" << endl;
    for (auto const& p : translations) {
	cout << setw(35) << left << "'" + p.first + "'" << p.second << endl;
    }
    cout << endl;
}

void Translator::print_deps(map<string*, vector<string*>> const& deps) {
    cout << setw(35) << left << "string"
	 << setw(15) << left << "deps" << endl;

    for (string* const s : schedule) {
	if (deps.find(s) != deps.end()) { 
	    cout << setw(35) << left << "'" + *s + "'";

	    cout << "[";
	    for (string* s : deps.at(s)) {
		cout << *s + ", ";
	    }
	    cout << "]" << endl;
	}
    }
}
