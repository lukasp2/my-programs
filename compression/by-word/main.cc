#include "translator.h"
#include <iostream>

int main() {
    Translator translator{"file.txt"};
    
    translator.encode();

    translator.decode("encoded_file.txt");

    return 0;
}
