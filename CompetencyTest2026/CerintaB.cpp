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

	bool operator==(const Option& other) const {
		return price == other.price && duration == other.duration &&
			brand == other.brand && category == other.category &&
			option == other.option;
	}
};

struct User {
	double maxPrice;
	int maxDuration;
	map<string, set<string>> categories;
};

struct Config {
	double totalPrice;
	int totalDuration;
	vector<Option> options;

	bool operator<(const Config& other) const {
		if (totalPrice != other.totalPrice) return totalPrice < other.totalPrice;
		if (totalDuration != other.totalDuration) return totalDuration < other.totalDuration;

		for (int i = 0; i < min(options.size(), other.options.size()); i++) {
			if (options[i].brand != other.options[i].brand) return options[i].brand < other.options[i].brand;
			if (options[i].category != other.options[i].category) return options[i].category < other.options[i].category;
			if (options[i].option != other.options[i].option) return options[i].option < other.options[i].option;
		}
		return options.size() < other.options.size();
	}

	bool operator==(const Config& other) const {
		if (totalPrice != other.totalPrice || totalDuration != other.totalDuration || options.size() != other.options.size())
			return false;
		for (int i = 0; i < options.size(); i++)
			if (!(options[i] == other.options[i])) return false;
		return true;
	}
};

int main(int argc, char** argv)
{
	// Keep this line and only modify the input name here
	const char* filename = (argc < 2) ? "IO/B/inputB4.txt" : argv[1];

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
			for (int k = 0; k < nrOptions; k++) {
				string optionName;
				fin >> optionName;
				U.categories[category].insert(optionName);
			}
		}
		users[username] = U;
	}

	sort(options.begin(), options.end());

	for (const auto& userPair : users) {
		const string& username = userPair.first;
		const User& user = userPair.second;
		cout << username << "\n";
		vector<Config> current_configs;
		current_configs.push_back({ 0.0, 0, {} });
		bool possible = true;

		for (const auto& catPair : user.categories) {
			const string& reqCategory = catPair.first;
			const set<string>& allowedOptions = catPair.second;
			vector<Option> catOptions;
			for (const auto& op : options) {
				if (op.category == reqCategory && allowedOptions.count(op.option)) {
					if (op.price <= user.maxPrice && op.duration <= user.maxDuration) {
						catOptions.push_back(op);
					}
				}
			}
			if (catOptions.empty()) {
				possible = false;
				break;
			}

			vector<Config> next_configs;
			for (const auto& partial_cfg : current_configs) {
				for (const auto& opt : catOptions) {
					double newPrice = partial_cfg.totalPrice + opt.price;
					int newDuration = max(partial_cfg.totalDuration, opt.duration);
					if (newPrice <= user.maxPrice && newDuration <= user.maxDuration) {
						Config new_cfg = partial_cfg;
						new_cfg.totalPrice = newPrice;
						new_cfg.totalDuration = newDuration;
						new_cfg.options.push_back(opt);
						next_configs.push_back(new_cfg);
					}
				}
			}
			current_configs = next_configs;
			if (current_configs.empty()) {
				possible = false;
				break;
			}
		}

		if (!possible || current_configs.empty()) {
			cout << "Imposibil\n";
		}
		else {
			for (auto& config : current_configs) {
				sort(config.options.begin(), config.options.end());
			}
			sort(current_configs.begin(), current_configs.end());
			current_configs.erase(unique(current_configs.begin(), current_configs.end()), current_configs.end());
			cout << current_configs.size() << "\n";
			for (const auto& config : current_configs) {
				cout << config.totalPrice << " " << config.totalDuration << "\n";
				for (const auto& it : config.options) {
					cout << it.brand << " " << it.category << " " << it.option << " " << it.price << " " << it.duration << "\n";
				}
			}
		}
	}

	return 0;
}
// Your code will end here
