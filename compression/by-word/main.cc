#include "translator.h"
#include <iostream>

using namespace std;
int main() {
    Translator translator{};
    
    translator.encode("f_1MB.txt");

    //translator.decode("f_1MB.txt");

    return 0;
}
