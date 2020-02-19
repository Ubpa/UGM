#include <iostream>

#include <UGM/Vec.h>

using namespace std;
using namespace Ubpa;

int main() {
	Vecf3 v(1, 2, 3);
	Vecf3 u = { 0,2,3 };
	cout << sizeof(Vecf3) << endl;

	// add
	cout << -v << endl;
	cout << v + u << endl;
	cout << v - u << endl;
	v += u;
	cout << v << endl;
	v -= u;
	cout << v << endl;

	// mul
	cout << v * 3.f << endl;
	cout << 4.f * v << endl;
	v *= 5.f;
	cout << v << endl;
	cout << v / 3.f << endl;
	v /= 5.f;
	cout << v << endl;

	Vecf3 vs[4];

	for (size_t i = 0; i < 4; i++)
		std::cin >> vs[i];

	std::cout << vs[2] + vs[3];
	std::cout << Vecf3(vs[0][0] + vs[1][0], vs[0][1] + vs[1][1], vs[0][2] + vs[1][2]);
}