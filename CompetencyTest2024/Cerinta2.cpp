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
	double quantity;
	int expiration_date;

	bool operator<(const Batch& other) const {
		return expiration_date < other.expiration_date;
	}
};

struct Product {
	double production_price;
	vector<Batch> batches;
};

struct Offer {
	string name;
	double offer_price;
	double offer_qty;
};

struct DayOffers {
	int offer_date;
	vector<Offer> offers;
};

map<string, map<string, double>> recipes;
map<string, map<string, double>> base_ingredients;
map<string, double> calculated_price;

void resolve_product(const string& name, map<string, Product>& inventory) {
	if (base_ingredients.find(name) != base_ingredients.end()) return;

	if (recipes.find(name) == recipes.end()) {
		base_ingredients[name][name] = 1.0;
		if (inventory.find(name) != inventory.end()) {
			calculated_price[name] = inventory[name].production_price;
		}
		return;
	}

	double total_price = 0;
	map<string, double> total_req;
	for (const auto& item : recipes[name]) {
		string comp_name = item.first;
		double comp_qty = item.second;

		resolve_product(comp_name, inventory);

		total_price += calculated_price[comp_name] * comp_qty;
		for (const auto& b_ing : base_ingredients[comp_name]) {
			total_req[b_ing.first] += b_ing.second * comp_qty;
		}
	}
	base_ingredients[name] = total_req;
	calculated_price[name] = total_price;
}

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
		int expiration_date;
		double quantity;
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
	vector<DayOffers> timeline;
	for (int i = 0; i < m; i++) {
		DayOffers day;
		int nr;
		fin >> day.offer_date >> nr;
		for (int j = 0; j < nr; j++) {
			Offer o;
			fin >> o.name >> o.offer_price >> o.offer_qty;
			day.offers.push_back(o);
		}
		timeline.push_back(day);
	}

	int p;
	if (fin >> p) {
		for (int i = 0; i < p; i++) {
			string r_name;
			int r_count;
			fin >> r_name >> r_count;
			for (int j = 0; j < r_count; j++) {
				string ing_name;
				double ing_qty;
				fin >> ing_name >> ing_qty;
				recipes[r_name][ing_name] = ing_qty;
			}
		}
	}

	double total = 0;
	vector<pair<string, double>> sales;

	for (const auto& day : timeline) {
		for (const auto& offer : day.offers) {
			resolve_product(offer.name, inventory);

			if (base_ingredients[offer.name].empty() || calculated_price.find(offer.name) == calculated_price.end())
				continue;

			if (offer.offer_price < calculated_price[offer.name])
				continue;

			double max_can_build = offer.offer_qty;

			for (const auto& ing : base_ingredients[offer.name]) {
				string ing_name = ing.first;
				double req_per_unit = ing.second;

				double available_valid_stock = 0;
				for (const auto& batch : inventory[ing_name].batches) {
					if (batch.expiration_date > day.offer_date) {
						available_valid_stock += batch.quantity;
					}
				}
				max_can_build = min(max_can_build, available_valid_stock / req_per_unit);
			}

			if (max_can_build > 0) {
				for (const auto& ing : base_ingredients[offer.name]) {
					string ing_name = ing.first;
					double total_needed = ing.second * max_can_build;

					for (auto& batch : inventory[ing_name].batches) {
						if (batch.expiration_date > day.offer_date && batch.quantity > 0) {
							double taken = min(batch.quantity, total_needed);
							batch.quantity -= taken;
							total_needed -= taken;
							if (total_needed <= 0) break;
						}
					}
				}

				sales.push_back({ offer.name, max_can_build });
				total += max_can_build * offer.offer_price;
			}
		}
	}

	for (auto& it : sales) {
		string display_name = it.first;
		replace(display_name.begin(), display_name.end(), '_', ' ');
		cout << display_name << " " << it.second << "\n";
	}
	cout << total;
	return 0;
}

// Your code will end here
