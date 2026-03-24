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

struct Problem {
	string id_problem;
	string speciality;
	int duration;
	int priority;

	bool operator<(const Problem& other) const{
		return priority < other.priority;
	}
};

struct Doctor {
	string id_doctor;
	set<string> speciality;
	int approach_time;
	queue<pair<int, string>> approached_problems;
};

int main(int argc, char** argv)
{
	// Keep this line and only modify the input name here
	const char* filename = (argc < 2) ? "IO/1/input2.txt" : argv[1];

	ifstream fin(filename);
	
	int n;
	fin >> n;
	priority_queue<Problem> problems;
	for (int i = 0; i < n; i++) {
		Problem p;
		fin >> p.id_problem >> p.speciality >> p.duration >> p.priority;
		problems.push(p);
	}

	int m;
	fin >> m;
	vector<Doctor> doctors;
	for (int i = 0; i < m; i++) {
		Doctor d;
		fin >> d.id_doctor;
		int nr;
		fin >> nr;
		for (int j = 0; j < nr; j++) {
			string speciality_name;
			fin >> speciality_name;
			d.speciality.insert(speciality_name);
		}
		d.approach_time = 9;
		doctors.push_back(d);
	}
	
	while (!problems.empty()) {
		Problem problem = problems.top();
		problems.pop();
		for (auto& dr : doctors) {
			if (dr.speciality.find(problem.speciality) != dr.speciality.end() && dr.approach_time + problem.duration <= 17) {
				dr.approached_problems.push({ dr.approach_time, problem.id_problem });
				dr.approach_time += problem.duration;
				break;
			}
		}
	}

	for (auto& dr : doctors) {
		if (!dr.approached_problems.empty()) {
			cout << dr.id_doctor << " " << dr.approached_problems.size() << " ";
			while (!dr.approached_problems.empty()) {
				cout << dr.approached_problems.front().second << " " << dr.approached_problems.front().first << " ";
				dr.approached_problems.pop();
			}
			cout << "\n";
		}
	}
	return 0;
}

// Your code will end here
