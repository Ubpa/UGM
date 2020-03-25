#include <iostream>

#include <UGM/ray.h>
#include <UGM/transform.h>

using namespace std;
using namespace Ubpa;

int main() {
	rayf3 r(pointf3{ 0.f }, vecf3{ 0.f,1.f,0.f });

	r.print();
	cout << "r: " << r << endl;
	cout << "o: " << r.point() << endl;
	cout << "d: " << r.dir() << endl;
	cout << "at(0.5f): " << r.at(0.5f) << endl;
	pointf3 p0{ -1.f,2.f,1.f };
	pointf3 p1{ 0.f, 2.f, -1.f };
	pointf3 p2{ 1.f,2.f,1.f };
	auto [isIntersect, wuv, t] = r.intersect(trianglef3{ p0,p1,p2 });
	cout << "intersect_triangle" << endl
		<< "  - p0: " << p0 << endl
		<< "  - p1: " << p1 << endl
		<< "  - p2: " << p2 << endl
		<< "  - isIntersect: " << isIntersect << endl
		<< "  - wuv: " << wuv[0] << ", " << wuv[1] << ", " << wuv[2] << endl
		<< "  - t: " << t << endl
		<< "  - wp0 + up1 + vp2: " << pointf3::combine(array<pointf3, 3>{p0, p1, p2}, wuv) << endl
		<< "  - ray.at(t): " << r.at(t) << endl;
	cout << "translate(vecf3{1.f}): " << transformf(vecf3{ 1.f }) * r << endl;

	return 0;
}
