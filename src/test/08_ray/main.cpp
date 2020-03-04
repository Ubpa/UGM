#include <iostream>

#include <UGM/ray.h>
#include <UGM/transform.h>

using namespace std;
using namespace Ubpa;

int main() {
	rayf3 r(pointf3{ 0.f }, vecf3{ 0.f,1.f,0.f });

	r.print();
	cout << "r: " << r << endl;
	cout << "o: " << r.o << endl;
	cout << "d: " << r.d << endl;
	cout << "at(0.5f): " << r.at(0.5f) << endl;
	pointf3 p0{ -1.f,2.f,1.f };
	pointf3 p1{ 0.f, 2.f, -1.f };
	pointf3 p2{ 1.f,2.f,1.f };
	auto [isIntersect, wuv, t] = r.intersect_triangle(p0,p1,p2);
	cout << "intersect_triangle" << endl
		<< "  - p0: " << p0 << endl
		<< "  - p1: " << p1 << endl
		<< "  - p2: " << p2 << endl
		<< "  - isIntersect: " << isIntersect << endl
		<< "  - wuv: " << get<0>(wuv) << ", " << get<1>(wuv) << ", " << get<2>(wuv) << endl
		<< "  - t: " << t << endl
		<< "  - wp0 + up1 + vp2: " << p0 + get<1>(wuv) * (p1 - p0) + get<2>(wuv) * (p2 - p0) << endl
		<< "  - ray(t): " << r.at(t) << endl;
	cout << "translate(vecf3{1.f}): " << transformf(vecf3{ 1.f }) * r << endl;

	return 0;
}
