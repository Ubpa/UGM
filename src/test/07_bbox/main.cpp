#include <iostream>

#include <UGM/bbox.h>
#include <UGM/val.h>

using namespace std;
using namespace Ubpa;

int main() {
	bboxf3 b;
	b.combine_with(pointf3{ 0,0,0 });
	b.combine_with(pointf3{ 1,2,3 });

	cout << "b" << b << endl;
	cout << "is_valid: " << b.is_valid() << endl;
	for (size_t i = 0; i < 8; i++)
		cout << "corner[i]: " << b.corner(i) << endl;
	cout << "center: " << b.center() << endl;
	cout << "diagonal: " << b.diagonal() << endl;
	cout << "area: " << b.area() << endl;
	cout << "volume: " << b.volume() << endl;
	cout << "max_extent: " << b.max_extent() << endl;
	cout << "lerp(valf3{0.2f}): " << b.lerp(valf3{ 0.2f }) << endl;
	cout << "offset(pointf3{1.f}): " << b.offset(pointf3{ 1.f }) << endl;

	bboxf3 c(pointf3{ 1,1,1 }, pointf3{ 0,2,2 }, nullptr_t{});
	cout << "c: " << c << endl;
	cout << "intersect(b,c): " << bboxf3::intersect(b, c) << endl;

	return 0;
}
