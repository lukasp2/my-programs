#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <string>
#include <map>
#include <vector>

class Translator {
public:
    Translator() {};

    ~Translator();
    
    void encode(std::string const filename);

    // helpers: encode()
    void get_strings_to_translate();
    void find_dependencies();
    void create_schedule();
    void create_translations();
    void write_file();
    
    // helpers: encode(): get_strings_to_translate()
    void get_all_strings(std::map<std::string, int>& strings, int series_size);
    void remove_obsolete_entries(std::string const& str, int const occurences);

    // helpers: encode(): create_translations()
    void replace_str_in_schedule(std::string* word, std::string const hash_val);
    void update_substrs_n_sprstrs();
    void refactor_schedule();
    // .... //

    void decode(std::string const filename);
    
    // additionals
    void print_schedule();
    void print_deps(std::map<std::string*, std::vector<std::string*>> const& deps);
    void print_translations();
    void print_file();
    
private:
    std::string filename{};

    // variables: encode(): get_strings_to_translate()
    std::map<std::string, std::string*> str_ptrs{};
    std::map<std::string*, int> occurences{};
    std::map<std::string*, int> lucrativity{};

    // variables: encode(): create_schedule()
    std::vector<std::string*> schedule{};

    // variables: encode(): find_dependencies()
    std::map<std::string*, std::vector<std::string*>> str_to_substr{};
    std::map<std::string*, std::vector<std::string*>> substr_to_str{};
    
    // variables: encode(): create_translations()
    std::vector<std::pair<std::string, std::string>> translations{};
};

#endif
