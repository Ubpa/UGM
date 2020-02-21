#include <iostream>

#include <UGM/mat.h>

using namespace std;
using namespace Ubpa;

int main() {
	matf3 m0;
	vecf3 c0 = { 1,2,3 };
	vecf3 c1 = { 4,5,6 };
	vecf3 c2 = { 7,8,9 };
	matf3 m1(c1, c2, c0);

	cin >> m0;
	cout << m0 << endl;
	m0.print();

	(m0 + m1).print();
	(m0 - m1).print();
	(3 * m0).print();
	(m0 * m1).print();
	cout << m0(0, 1) << endl;
	m0.inverse().print();
	matf4 m4;
	cin >> m4;
	m4.inverse().print();
}
