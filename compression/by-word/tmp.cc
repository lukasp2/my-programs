#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

// in string str: replace substring "from" with substring "to"
void replaceAll(string& str, string const& from_str, string const& to_str) {
    if(from_str.empty())
        return;

    auto substr_pos = str.find(from_str, 0);
    while(substr_pos != std::string::npos) {
	cout << "b4 " << str << endl;
        str.replace(substr_pos, from_str.length(), to_str);
	//replace(str.begin(), str.end(), from_str, to_str);
	cout << "after: " << str << endl;
	
	// in case 'to' contains 'from', like replacing 'x' with 'yx'
        substr_pos += to_str.length();

	substr_pos = str.find(from_str, substr_pos);
    }
}

int main() {
    string str = "hello there";
    string from_str = " th";
    string to_str = "!";

    replaceAll(str, from_str, to_str);
    
    cout << str << endl;
    
    return 0;
}

