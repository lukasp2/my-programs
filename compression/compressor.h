#ifndef COMP_H
#define COMP_H

#include <utility>
#include <string>
#include <map>
#include <vector>
#include <iterator>

typedef std::istreambuf_iterator<char> char_it;

class Compressor {
public:
    Compressor() {};

    ~Compressor();
    
    Compressor(Compressor const& c) = delete;

    void compress(std::string const filename);

    void decode();
    
    void print(int i) const;

private:
    int lookahead = 16;
    
    std::vector<char> insertion_order{};

    std::map<char, int> char_positions{};

    std::map<char, std::vector<int>*> char_map{};
};

#endif
