#include <iostream>

#include <UGM/transform.h>

using namespace std;
using namespace Ubpa;

int main() {
	eulerf e(to_radian(13.f), to_radian(42.f), to_radian(71.f));
	cout << e << endl;
	cout << e.to_quat().to_euler() << endl;
	cout << transformf(e).decompose_euler() << endl;

	quatf q({ 1,1,1 }, to_radian(45.f));
	cout << q << endl;
	cout << transformf(q).decompose_quatenion() << endl;

	cout << transformf(q).decompose_euler() << endl;
	cout << q.to_euler() << endl;

	transformf::rotate_with<Axis::X>(to_radian(30.f)).print();
	transformf::rotate_with<Axis::Y>(to_radian(30.f)).print();
	transformf::rotate_with<Axis::Z>(to_radian(30.f)).print();
}
