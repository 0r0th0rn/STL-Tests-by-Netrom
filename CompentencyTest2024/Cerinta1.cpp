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

struct Batch {
	int quantity;
	int expiration_date;

	bool operator<(const Batch& other) const {
		return expiration_date < other.expiration_date;
	}
};

struct Product {
	double production_price;
	vector<Batch> batches;
};

int main(int argc, char** argv)
{
	// Keep this line and only modify the input name here
	const char* filename = (argc < 2) ? "IO/input3.txt" : argv[1];

	ifstream fin(filename);
	int n;
	fin >> n;
	map<string, Product> inventory;
	for (int i = 0; i < n; i++) {
		string name;
		fin >> name;
		int expiration_date, quantity;
		double price;
		fin >> price >> quantity >> expiration_date;
		if (inventory.find(name) == inventory.end())
			inventory[name].production_price = price;
		inventory[name].batches.push_back({ quantity, expiration_date });
	}

	for (auto& it : inventory) {
		sort(it.second.batches.begin(), it.second.batches.end());
	}

	int m;
	fin >> m;
	double total=0;
	vector<pair<string, int>> sales;
	for (int i = 0; i < m; i++) {
		int offer_date, nr;
		fin >> offer_date >> nr;
		for (int j = 0; j < nr; j++) {
			string name;
			double offer_price;
			int offer_qty;
			fin >> name >> offer_price >> offer_qty;
			if (inventory.find(name) == inventory.end())
				continue;
			if (offer_price < inventory[name].production_price)
				continue;
			Product& current_product = inventory[name];
			int sold_qty = 0;
			for (auto& batch : current_product.batches) {
				if (batch.quantity == 0)
					continue;
				if (batch.expiration_date <= offer_date) {
					batch.quantity = 0;
					continue;
				}
				if (offer_qty == 0)
					break;
				int min1 = min(batch.quantity, offer_qty);
				batch.quantity -= min1;
				offer_qty -= min1;
				sold_qty += min1;
			}
			if (sold_qty > 0) {
				sales.push_back({ name, sold_qty });
				total += sold_qty * offer_price;
			}
		}
	}

	for (const auto& it : sales) {
		cout << it.first << " " << it.second << "\n";
	}
	cout << total;
	return 0;
}

// Your code will end here
