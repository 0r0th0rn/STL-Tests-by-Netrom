#include <iostream> // Input/Output
#include <fstream>
#include <sstream>
#include <string>
#include <vector> // Containers
#include <array>
#include <list>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> // Algorithms & Iterators
#include <iterator>
#include <numeric>
#include <functional>
#include <memory> // Memory & Smart Pointers
#include <tuple> // Utility
#include <limits>
#if __has_include(<ranges>)
#include <ranges>
#endif
#include <cmath> // Math & Complex Numbers
#include <complex>
#include <regex> // Miscellaneous
#include <valarray>

// Your code will start here
using namespace std;

struct Option {
	string option;
	string category;
	string brand;
	double price;
	int duration;

	bool operator<(const Option& other) const {
		if (price != other.price) return price < other.price;
		if (duration != other.duration) return duration < other.duration;
		if (brand != other.brand) return brand < other.brand;
		if (category != other.category) return category < other.category;
		return option < other.option;
	}
};

struct User {
	double maxPrice;
	int maxDuration;
	map<string, string> categories;
};

int main(int argc, char** argv)
{
	// Keep this line and only modify the input name here
	const char* filename = (argc < 2) ? "IO/A/inputA4.txt" : argv[1];

	ifstream fin(filename);

	int n;
	if (!(fin >> n)) return 0;

	vector<Option> options;
	for (int i = 0; i < n; i++) {
		Option O;
		fin >> O.option >> O.category >> O.brand >> O.price >> O.duration;
		options.push_back(O);
	}

	int m;
	fin >> m;
	map<string, User> users;
	for (int i = 0; i < m; i++) {
		string username;
		User U;
		fin >> username >> U.maxPrice >> U.maxDuration;
		int nr;
		fin >> nr;
		for (int j = 0; j < nr; j++) {
			string category;
			fin >> category;
			int nrOptions;
			fin >> nrOptions;
			string optionName;
			fin >> optionName;
			U.categories[category] = optionName;
		}
		users[username] = U;
	}

	sort(options.begin(), options.end());

	for (const auto& userPair : users) {
		const string& username = userPair.first;
		const User& user = userPair.second;
		double sumPrice = 0.0;
		vector<Option> ans;
		for (const auto& catPair : user.categories) {
			const string& reqCategory = catPair.first;
			const string& reqOption = catPair.second;
			for (const auto& op : options) {
				if (op.option == reqOption && op.category == reqCategory && op.duration <= user.maxDuration) {
					sumPrice += op.price;
					ans.push_back(op);
					break;
				}
			}
		}

		cout << username << "\n";
		if (sumPrice > user.maxPrice || ans.size() != user.categories.size()) {
			cout << "Imposibil\n";
		}
		else {
			sort(ans.begin(), ans.end());
			for (const auto& it : ans) {
				cout << it.brand << " " << it.category << " " << it.option << " " << it.price << " " << it.duration << "\n";
			}
		}
	}

	return 0;
}
// Your code will end here
