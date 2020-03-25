#include <iostream>

#include <UGM/line.h>
#include <UGM/transform.h>

using namespace std;
using namespace Ubpa;

int main() {
	linef3 l(pointf3{ 0.f }, vecf3{ 0.f,1.f,0.f });

	l.print();
	cout << "l: " << l << endl;
	cout << "pnt: " << l.point() << endl;
	cout << "d: " << l.dir() << endl;
	cout << "at(0.5f): " << l.at(0.5f) << endl;
	pointf3 p0{ -1.f,2.f,1.f };
	pointf3 p1{ 0.f, 2.f, -1.f };
	pointf3 p2{ 1.f,2.f,1.f };
	auto [isIntersect, wuv, t] = l.intersect(trianglef3{ p0, p1, p2 });
	cout << "intersect_triangle" << endl
		<< "  - p0: " << p0 << endl
		<< "  - p1: " << p1 << endl
		<< "  - p2: " << p2 << endl
		<< "  - isIntersect: " << isIntersect << endl
		<< "  - wuv: " << wuv[0] << ", " << wuv[1] << ", " << wuv[2] << endl
		<< "  - t: " << t << endl
		<< "  - wp0 + up1 + vp2: " << pointf3::combine(array<pointf3, 3>{p0,p1,p2}, wuv) << endl
		<< "  - line.at(t): " << l.at(t) << endl;
	cout << "translate(vecf3{1.f}): " << transformf(vecf3{ 1.f }) * l << endl;

	cout << l.move(pointf3{ 1.f }) << endl;
	cout << l + vecf3{ 2.f } << endl;

	return 0;
}
