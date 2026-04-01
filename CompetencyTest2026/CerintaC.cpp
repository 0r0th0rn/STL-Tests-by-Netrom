#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <functional>
#include <memory>
#include <tuple>
#include <limits>
#if __has_include(<ranges>)
#include <ranges>
#endif
#include <cmath>
#include <complex>
#include <regex>
#include <valarray>

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
	map<string, set<string>> categories;
};

int main(int argc, char** argv)
{
	const char* filename = (argc < 2) ? "IO/C/inputC4.txt" : argv[1];

	ifstream fin(filename);

	int n;
	fin >> n;
	
	set<pair<string, string>> unique_opts;
	vector<Option> options;
	
	for (int i = 0; i < n; i++) {
		Option O;
		fin >> O.option >> O.category >> O.brand >> O.price >> O.duration;
		options.push_back(O);
		unique_opts.insert({ O.category, O.option });
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
			for (int k = 0; k < nrOptions; k++) {
				string optionName;
				fin >> optionName;
				U.categories[category].insert(optionName);
				unique_opts.insert({ category, optionName });
			}
		}
		users[username] = U;
	}
	
	int r;
	fin >> r;
	vector<pair<string, string>> queries(r);
	for (int i = 0; i < r; i++) {
		fin >> queries[i].first >> queries[i].second;
	}
		
	sort(queries.begin(), queries.end());

	for (int i = 0; i < (int)queries.size(); i++) {
		string targetCategory = queries[i].first;
		string targetOption = queries[i].second;
		int max_relevance = 0;
		vector<pair<string, string>> best_matches;
		
		for (auto it = unique_opts.begin(); it != unique_opts.end(); it++) {
			string candCategory = it->first;
			string candOption = it->second;
			if (targetCategory == candCategory) continue;

			int current_relevance = 0;
			
			for (auto userIt = users.begin(); userIt != users.end(); userIt++) {
				const User& user = userIt->second;
				auto itTarget = user.categories.find(targetCategory);
				auto itCand = user.categories.find(candCategory);
				
				if (itTarget != user.categories.end() && itTarget->second.count(targetOption) &&
					itCand != user.categories.end() && itCand->second.count(candOption)) {
					current_relevance += 1;
				}
			}

			if (current_relevance > 0) {
				if (current_relevance > max_relevance) {
					max_relevance = current_relevance;
					best_matches.clear();
					best_matches.push_back(*it);
				}
				else if (current_relevance == max_relevance) {
					best_matches.push_back(*it);
				}
			}
		}

		if (max_relevance == 0) {
			cout << targetCategory << " " << targetOption << " 0\nNo stats\n";
		}
		else {
			cout << targetCategory << " " << targetOption << " " << (int)best_matches.size() << "\n";
			for (int j = 0; j < (int)best_matches.size(); j++) {
				cout << best_matches[j].first << " " << best_matches[j].second << " " << max_relevance << "\n";
			}
		}
	}
	
	return 0;
}
