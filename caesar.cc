#include <iostream>
#include <string>

bool lowercase(char c) {
    return ( int(c) >= 97 && int(c) <= 122 );
}

bool uppercase(char c) {
    return ( int(c) >= 65 && int(c) <= 90 );
}

char rotate_char(char c, int rotation) {

    char rot_char{ char(int(c) + rotation) };
    
    while (lowercase(c) && !lowercase(rot_char)) {
	rot_char -= 26;
    }

    while (uppercase(c) && !uppercase(rot_char)) {
	rot_char -= 26;
    }
	
    return rot_char;
}

int main(int argc, char** argv) {

    if (argc != 2)
	std::cout << "Usage: ./caesar [rotation]" << std::endl;
    
    int rotation{ std::stoi(argv[1]) };
    
    std::string output{};	
    
    while (std::cin) {
	char c = std::cin.get();
	
	if ( uppercase(c) || lowercase(c) )
	    c = rotate_char(c, rotation);

	if (c != EOF)
	    std::cout << c;
    }
    
    return 0;
}
