#include <iostream>

#include <UGM/transform.h>

using namespace std;
using namespace Ubpa;

int main() {
	transformf tsfm; // default constructor

	eulerf e(to_radian(13.f), to_radian(42.f), to_radian(71.f));
	transformf t(e);
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

	cout << transformf::look_at(pointf3{0.f}, pointf3{0.f,0.f,1.f}) << endl;
	cout << transformf::perspective(to_radian(60.f), 16.f / 9.f, 0.1f, 100.f) << endl;
	cout << transformf::orthographic(10.f, 10.f, 0.1f, 100.f) << endl;

	cout << t.decompose_translation() << endl;
	cout << t.decompose_mat3() << endl;
	cout << t.decompose_scale() << endl;
	cout << t.decompose_euler() << endl;
	cout << t.decompose_mat3() << endl;
	cout << t.decompose_rotation_matrix() << endl;

	cout << t * transformf(q) << endl;
	cout << t * pointf3{} << endl;
	cout << t * vecf3{} << endl;
	cout << t * normalf{} << endl;

	cout << transformf::rotate_with<Axis::X>(to_radian(180.f)).decompose_quatenion() << endl;
	auto trs = transformf{ vecf3{1.f}, quatf{vecf3{1,1,1},to_radian(45.f)}, scalef3{2.f,3.f,4.f} };
	cout << trs * trs.inverse_sim() << endl;

	matf2 m2{ 2,9,5,3 };
	auto [U2, S2, V2] = m2.SVD();
	U2.print();
	S2.print();
	V2.print();

	matf3 m3{
		-0.558253, -0.0461681, -0.505735,
		-0.411397,  0.0365854,  0.199707,
		 0.285389, -0.3137890,  0.200189
	};
	auto [U3, S3, V3] = m3.SVD();
	U3.print();
	S3.print();
	V3.print();

	matf2 s2{ 1,0,0,-1 };
	auto [sU2, sS2, sV2] = s2.SVD();
	sU2.print();
	sS2.print();
	sV2.print();
	auto [pU2, pS2, pV2] = s2.signed_SVD();
	pU2.print();
	pS2.print();
	pV2.print();
}
