#include "translator.h"
#include <iostream>

using namespace std;
int main() {
    Translator translator{};
    
    translator.encode("file.txt");

    cout << "ENCODED FILE" << endl;
    translator.print_file();

    translator.decode("file.txt");

    cout << "DECODED FILE" << endl;
    translator.print_file();

    return 0;
}
