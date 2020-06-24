#include "translator.h"

int main() {
    Translator translator{};

    translator.encode("file.txt");

    translator.decode("encoded_file.txt");

    return 0;
}
