#include <iostream>

#include <UGM/transform.h>

using namespace std;
using namespace Ubpa;

int main() {
	auto m = matf4::eye();
	transformf t(m);
	t(3, 3) = 2;
	t.print();
	transformf(vecf3(0, 1, 0)).print(); // translation
	transformf(scalef3(0, 1, 0)).print(); // translation
	transformf(vecf3(0, 1, 0), 3.1415926f).print(); // rotation
	transformf(quatf(1, { 0, 0, 0 })).print();

	pointf3 pos(0, 0, 1);
	pointf3 target(0, 0, 0);
	transformf::look_at(pos, target).print();

	transformf::orthographic(10.f, 10.f, 0.1f, 100.f).print();
	constexpr float fovY = to_radian(50.f);
	transformf::perspcetive(fovY, 16.f / 9.f, 0.1f, 50.f).print();
}
