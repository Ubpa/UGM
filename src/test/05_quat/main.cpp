#include <iostream>

#include <UGM/quat.h>

using namespace std;
using namespace Ubpa;

int main() {
	vecf3 axis(0, 1, 0);
	float angle = 45.f;
	quatf q(axis, angle / 180 * 3.1415926);
	pointf3 p(1, 0, 0);
	cout << q * p << endl;
}
