#include <iostream>

#include <UGM/mat.h>

using namespace std;
using namespace Ubpa;

int main() {
	vecf3 c0 = { 1,2,3 };
	vecf3 c1 = { 4,5,6 };
	vecf3 c2 = { 7,8,9 };
	matf3 m0(c0, c1, c2);
	matf3 m1(c1, c2, c0);
	cout << m0 << endl;
	cout << m0 + m1 << endl;
	cout << m0 - m1 << endl;
	cout << 3 * m0 << endl;
	cout << m0 * m1 << endl;
	cin >> m0;
	cout << m0;
	cout << m0(0, 1) << endl;
	cout << m0.inverse();
	matf4 m4;
	cin >> m4;
	cout << m4.inverse();
}
