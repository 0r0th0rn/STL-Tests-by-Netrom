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

struct Point {
	int x, y;
};

struct Button {
	Point top_left, bottom_right;
};

bool check(Point click, Button B) {
	return (click.x >= B.top_left.x && click.x <= B.bottom_right.x && click.y >= B.top_left.y && click.y <= B.bottom_right.y);
}

int main(int argc, char** argv)
{
	// Keep this line and only modify the input name here
	const char* filename = (argc < 2) ? "Challenge C/input_3.txt" : argv[1];

	ifstream fin(filename);

	int L, C;
	fin >> L >> C;

	map<string, Button> buttons;
	int n;
	fin >> n;
	for (int i = 0; i < n; i++) {
		string button_name;
		fin >> button_name;
		Button B;
		fin >> B.top_left.x >> B.top_left.y >> B.bottom_right.x >> B.bottom_right.y;
		buttons[button_name] = B;
	}

	int m;
	fin >> m;
	map<string, vector<Point>> users;
	for (int i = 0; i < m; i++) {
		string user_name;
		fin >> user_name;
		Point click;
		fin >> click.x >> click.y;
		users[user_name].push_back(click);
	}

	map<string, set<string>> pressed_buttons;
	map<string, vector<string>> users_pressed_buttons;
	for (auto user : users) {
		for (auto click : user.second) {
			for (auto button : buttons) {
				if (check(click, button.second)) {
					pressed_buttons[button.first].insert(user.first);
					users_pressed_buttons[user.first].push_back(button.first);
					break;
				}
			}
		}
	}

	map<string, map<string, int>> freq;
	for (auto user : users_pressed_buttons) {
		for (int i = 1; i < user.second.size(); i++) {
			if (user.second[i - 1] != user.second[i])
				freq[user.second[i - 1]][user.second[i]]++;
		}
	}

	string first_button;
	fin >> first_button;
	vector<string> main_flow;
	set<string> visited;

	string current = first_button;
	main_flow.push_back(current);
	visited.insert(current);

	while (true) {
		int max1 = -1;
		string next_button = "";
		for (auto it : freq[current]) {
			if (visited.find(it.first) == visited.end()) {
				if (max1 < it.second) {
					max1 = it.second;
					next_button = it.first;
				}
			}
		}

		if (next_button == "") {
			break;
		}
		current = next_button;
		visited.insert(current);
		main_flow.push_back(current);
	}

	for (int i = 0; i < main_flow.size(); i++)
		cout << main_flow[i] << " ";
	cout << endl;

	for (auto user : users_pressed_buttons) {
		int max_len = 0;
		int start = -1;
		int end = -1;
		for (int i = 0; i < user.second.size(); i++) {
			for (int j = 0; j < main_flow.size(); j++) {
				int k = 0;
				while (i + k < user.second.size() && j + k < main_flow.size() && user.second[i + k] == main_flow[j + k]) {
					k++;
				}
				if (k > max_len) {
					max_len = k;
					start = i;
					end = i + k - 1;
				}
			}
		}
		if (max_len >= 3) {
			cout << user.first << " ";
			for (string button : user.second)
				cout << button << " ";
			cout << start << " " << end << endl;
		}
	}

	//cerinta B
	//map<string, string> users_last_button;
	//int p;
	//fin >> p;
	//for (int i = 0; i < p; i++) {
	//	string username;
	//	fin >> username;
	//	Point click;
	//	fin >> click.x >> click.y;
	//	for (auto button : buttons) {
	//		if (check(click, button.second)) {
	//			users_last_button[username] = button.first;
	//			break;
	//		}
	//	}
	//}

	//for (auto user : users_last_button) {
	//	int max1 = -1;
	//	string button_max = "";
	//	for(auto it : freq[user.second])
	//		if (max1 < it.second) {
	//			max1 = it.second;
	//			button_max = it.first;
	//		}
	//	cout << user.first << ": " << button_max << endl;
	//}

	//cerinta A
	//for (auto button : pressed_buttons) {
	//	cout << button.first << ": ";
	//	for (auto user : button.second)
	//		cout << user << " ";
	//	cout << endl;
	//}

	return 0;
}

// Your code will end here
