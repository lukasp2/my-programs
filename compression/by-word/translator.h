#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <utility>
#include <string>
#include <map>
#include <vector>
#include <set>

#include <iostream>

class Translator {
public:
    Translator() {}

    void encode(std::string const filename);

    void decode(std::string const filename);
};

// the translated value
class Hash {
public:
    Hash(std::string filename) : filename{filename} {};

    Hash& operator++();

    void incr_pos(int idx);
	
    std::string get();

    void get_next();
    
private:
    // strings that are not used in the text file and can be used as aliases
    std::vector<std::string> free_strings{};

    std::string filename{};
    
    std::vector<int> ascii_values{32};
};

#endif
