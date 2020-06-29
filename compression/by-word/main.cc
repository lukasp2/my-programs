#include "translator.h"
#include <iostream>

int main() {
    Translator translator{};
    
    translator.encode("file.txt");

    translator.decode("encoded_file.txt");

    return 0;
}
