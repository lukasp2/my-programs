#include "compressor.h"
#include <iostream>

int main() {
    Compressor compressor{};

    compressor.compress("file.txt");

    compressor.print(100);

    compressor.decode();
    
    return 0;
}
