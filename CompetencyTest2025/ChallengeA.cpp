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
	const char* filename = (argc < 2) ? "Challenge A/input_3.txt" : argv[1];

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
	for (auto user : users) {
		for (auto click : user.second) {
			for (auto button : buttons) {
				if (check(click, button.second)) {
					pressed_buttons[button.first].insert(user.first);
					break;
				}
			}
		}
	}

	for (auto button : pressed_buttons) {
		cout << button.first << ": ";
		for (auto user : button.second)
			cout << user << " ";
		cout << endl;
	}

	return 0;
}

// Your code will end here
