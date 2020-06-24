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
    Hash() {};

    Hash& operator++();

    void incr_pos(int idx);
	
    std::string get();

    void print_asciis() {std::cout << "["; for (int i : ascii_values) std::cout << i << " "; std::cout << "] "; };

private:
    std::vector<int> ascii_values = { 0 };
};

#endif
