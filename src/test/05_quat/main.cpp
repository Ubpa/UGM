#include <iostream>

#include <UGM/quat.h>
#include <UGM/euler.h>

using namespace std;
using namespace Ubpa;

int main() {
	vecf3 axis(0, 1, 0);
	float angle = 45.f;
	quatf q(axis, to_radian(angle));
	quatf test = quatf(axis, angle);
	cout << q.axis() << endl;
	cout << q.theta() << endl;
	quatf x({1,1,1}, 0);
	pointf3 p(1, 0, 0);
	cout << q * p << endl;
	cout << quatf::identity() << endl;
	cout << q.theta() << endl;
	cout << q.is_identity() << endl;
	cout << quatf::rotate_with<Axis::X>(to_radian(45.f)) << endl;
	cout << eulerf(to_radian(45.f), to_radian(45.f), to_radian(45.f)).to_quat() << endl;
}
