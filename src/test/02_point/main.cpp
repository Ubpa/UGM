#include <iostream>

#include <UGM/point.hpp>

#include <vector>
#include <array>

using namespace std;
using namespace Ubpa;

int main() {
	pointf3 o(1, 1, 1);
	pointf3 p(2, 2, 2);
	vecf3 v(1, 2, 3);
	cout << o << endl;
	cout << p + v << endl;
	cout << p - v << endl;
	o += v;
	cout << o << endl;
	o -= v;
	cout << o << endl;
	cout << p - o << endl;
	cout << pointf3::distance(p, o) << endl;

	pointi3 ip(3, 3, 3);
	cout << ip << endl;
	cout << pointf3::distance(o, o + v) << endl;

	vector<pointf3> points;
	points.push_back(o);
	points.push_back(p);
	cout << pointf3::combine(points, 0.5f) << endl;
	array<float, 2> weights = { 0.25f,0.75f };

	cout << pointf3::combine(points, weights) << endl;
}
