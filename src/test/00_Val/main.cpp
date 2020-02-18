#include <iostream>

#include <UGM/Val.h>

using namespace std;
using namespace Ubpa;

int main() {
	Valf3 v(1, 2, 3);
	Valf3 u = { 4,5,6 };
	// Valf3 w = { 1,2 }; // error
	cout << v << endl;
	cin >> v;
	cout << v << endl;
	cout << u << endl;
}
