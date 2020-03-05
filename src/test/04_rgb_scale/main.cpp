#include <iostream>

#include <UGM/rgba.h>
#include <UGM/scale.h>

using namespace std;
using namespace Ubpa;

int main() {
	rgbf white = { 1.f,1.f,1.f };
	rgbf black = { 0.f,0.f,0.f };
	rgbaf a = { 1.f,1.f,1.f,0.5f };
	rgbaf b(white, 0.5f);
	rgbaf c(white);

	scalef3 s = { 0.4f,0.5f,0.6f };
	cout << white << endl;
	cout << white * black << endl;
	rgbf d{ 1.f,2.f,3.f };
	d *= white;
	cout << d << endl;
	cout << c.over(b) << endl;
	cout << c.over(b).to_rgb() << endl;
	cout << s;
	cout << d.illumination() << endl;
	cout << d.gray() << endl;

	return 0;
}
