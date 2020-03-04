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

	//cin >> m0;
	cout << m0 << endl;
	m0.print();

	(m0 + m1).print();
	(m0 - m1).print();
	(3 * m0).print();
	(m0 * m1).print();
	cout << m0(0, 1) << endl;
	cout << m0.at(2, 2);
	matf4 m4;
	cin >> m4;
	m4.inverse().print();

	std::array<float, 9> arr;
	for (auto& e : arr)
		cin >> e;

	cout << matf3(arr) << endl;
	auto m3 = matf3::eye();
	cout << m0 * m3 << endl;

	cout << m3.trace() << endl;

	cout << m3.transpose() << endl;

	cout << m3 * vecf3(2, 1, 0) << endl;
	cout << m4 * vecf4(2, 1, 0, 1) << endl;

	cout << m3.has_nan() << endl;
	cout << m3.is_all_zero() << endl;
	cout << m3.rmv_epsilon() << endl;

	cout << m3.data() << endl;
}
