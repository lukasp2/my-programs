#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <string>
#include <map>
#include <vector>

class Translator {
public:
    Translator(std::string filename) : filename{ filename } {}

    ~Translator();
    
    void encode();

    // helpers: encode()
    void get_strings_to_translate();

    // helpers: encode(): get_strings_to_translate()
    void get_all_strings(std::map<std::string, int>& strings, int series_size);
    void remove_obsolete_entries(std::string const& str, int const occurences);

    // helpers: encode()
    void find_dependencies();
    void create_schedule();
    void create_translations();

    // helpers: encode(): create_translations()
    void replace_str_in_schedule(int schedule_pos, std::string const hash_val);
    void update_substrs_n_sprstrs();
    void refactor_schedule();
    // .... //

    void decode(std::string filename);
    
    // helpers: decode()
    void write_file();
    // .... //

    // additionals
    void print_schedule();
    void print_deps(std::map<std::string*, std::vector<std::string*>> const& deps);
    void print_translations();
    
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
