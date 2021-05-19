#include <iostream>

#include <UGM/quat.hpp>
#include <UGM/euler.hpp>

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
	vecf3 v(1, 0, 0);
	cout << q * v << endl;
	cout << quatf::identity() << endl;
	cout << q.theta() << endl;
	cout << q.is_identity() << endl;
	cout << quatf::rotate_with<Axis::X>(to_radian(45.f)) << endl;
	cout << eulerf{ 45.f }.to_quat() << endl;
	quatf p(vecf3{0,0,1}, 2 * to_radian(angle));
	// verify: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
	cout << q << endl;
	cout << p << endl;
	cout << q.slerp(p, 0.5f) << endl;
}
