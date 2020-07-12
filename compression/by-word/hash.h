#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>

class Hash {
public:
    Hash(std::string filename) : filename{filename} {};

    Hash& operator++();

    void incr_pos(int idx);

    // returns current hash value
    std::string get();

    // increments hash value
    void get_next();
    
private:
    std::string filename{};

    // strings that are not used in the text file and can be used as aliases
    std::vector<std::string> free_strings{};
    
    std::vector<int> ascii_values{ 0 };
};

#endif
