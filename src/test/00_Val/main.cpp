#include <iostream>

#include <UGM/val.h>

using namespace std;
using namespace Ubpa;

int main() {
	valf3 v(1, 2, 3);
	valf3 u = { 0,2,3 };
	cout << sizeof(valf3) << endl;
	cout << (u == v ? "u==v" : "u!=v") << endl;
	cout << (u < v ? "u<v" : "u>=v") << endl;
	cout << v << endl;
	cin >> v;
	cout << v << endl;
	cout << u << endl;
	//cout << v.abs() << endl;
	cout << valf3(abs(v[0]), abs(v[1]), abs(v[2])) << endl;
	//cout << v.max_dim() << endl;
}