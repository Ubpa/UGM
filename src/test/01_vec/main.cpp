#include <iostream>

#include <UGM/vec.h>
#include <UGM/val.h>
#include <UGM/normal.h>

#include <array>

using namespace std;
using namespace Ubpa;

int main() {
	vecf3 v(1, 2, 3);
	vecf3 u = { 0,2,3 };
	vecf3 w(1.f);
	cout << sizeof(vecf3) << endl;

	// add
	cout << -v << endl;
	cout << v + u << endl;
	cout << v - u << endl;
	v += u;
	cout << v << endl;
	v -= u;
	cout << v << endl;
	cout << v * 5 << endl;
	cout << 5 * v << endl;

	// mul
	cout << v * 3.f << endl;
	cout << 4.f * v << endl;
	v *= 5.f;
	cout << v << endl;
	cout << v / 3.f << endl;
	v /= 5.f;
	cout << v << endl;

	// inner product
	cout << v.dot(u) << endl;
	cout << vecf3::dot(v, u) << endl;
	
	// norm
	cout << v.norm2() << endl;
	cout << v.norm() << endl;
	cout << v.normalize() << endl;
	v.normalize_self();
	cout << v.is_normalized() << endl;
	cout << v[2] << endl;

	// angle
	cout << vecf3::cos_theta(v, u) << endl;
	cout << v.cos_theta(u) << endl;

	// cross
	cout << vecf3::cross(v, u) << endl;
	cout << v.cross(u) << endl;

	// cast
	cout << v.cast_to<valf3>() << endl;

	// lerp
	cout << vecf3::lerp(v, u, 0.5) << endl;
	cout << v.lerp(u, 0.5f) << endl;

	// veci
	veci3 iv{ 1,2,3 };
	cout << iv * 5 << endl;
	cout << iv * 7 << endl;

	// normal
	normalf n{ 1,1,1 };
	normalf::reflect(n, n);
	normalf::refract(n, n, 0.f);
	n.coord_space();

	// project, orthogonalize
	cout << v.project(u.normalize()) << endl;
	cout << v.orthogonalize(u.normalize()) << endl;

	vecf3 vs[4];

	std::array<float, 3> float3;

	for (size_t i = 0; i < 4; i++)
		std::cin >> vs[i];

	std::cout << vs[2] + vs[3];
	std::cout << vecf3(vs[0][0] + vs[1][0], vs[0][1] + vs[1][1], vs[0][2] + vs[1][2]);
}