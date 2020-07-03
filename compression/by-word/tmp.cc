#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;
int main() {

    vector<pair<char, int>> v{};
    char a = 'a';
    for (int i{}; i < 8; ++i) {
	v.push_back(make_pair(a++, i));
    }
    
    map<string, int> m;
    m["a"] = 41;
    m["b"] = 45;
    m["c"] = 1;
    m["d"] = 4;
    m["e"] = 29;
    m["f"] = 67;

    for (auto p : m) cout << p.first << " : " << p.second << endl;

    sort(m.begin(), m.end(),
	 [](auto p1, auto p2){
	     return p1.second > p2.second;
	 });

    for (auto p : m) cout << p.first << " : " << p.second << endl;
    
    return 0;
}

