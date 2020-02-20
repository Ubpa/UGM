#include <iostream>

#include <UGM/Point.h>

using namespace std;
using namespace Ubpa;

int main() {
	Pointf3 o(1, 1, 1);
	Pointf3 p(2, 2, 2);
	Vecf3 v(1, 2, 3);
	cout << o << endl;
	cout << p + v << endl;
	cout << p - v << endl;
	o += v;
	cout << o << endl;
	o -= v;
	cout << o << endl;
	cout << p - o << endl;
}