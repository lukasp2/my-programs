#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <map>

using namespace std;
int main(int argc, char** argv) {

	// open file
	ifstream ifs;
	ifs.open( argv[1] );

	if (!ifs) {
		cout << "Unable to open file" << endl;
		exit(1);
	}

	// read file, fill map
	std::map<char, int> char_map{};
	char c;

	while(ifs >> c) {
		char_map[c]++;
	}

	ifs.close();

	// copy map to vector
	std::vector< std::pair<char, int> > char_vector{};

	for (auto const it : char_map) {
		char_vector.push_back(std::make_pair(it.first, it.second));
	}

	std::sort(char_vector.begin(), char_vector.end(), [](auto& a, auto& b){ return a.second > b.second; });

	int total{};
	for (auto it : char_vector) {
		cout << it.first << " " << it.second << endl;
		total += it.second;
	}

	cout << "total:" << total << endl;

	return 0;
}
