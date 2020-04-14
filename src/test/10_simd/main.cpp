#include <iostream>
#include <UGM/vec.h>
#include <UGM/point.h>
#include <UGM/mat.h>
//#include <UGM/scale.h>

using namespace Ubpa;
using namespace std;

int main(int argc, char* argv[])
{
    vecf3 test;
    vecf4 a;
    vecf4 b;
    cin >> a >> b;
    cout << a + b << endl;
    cout << a.dot(b) << endl;
    pointf4 p0, p1;
    cin >> p0 >> p1;
    p0 -= a;
    cout << p0 - p1 << endl;
    matf4 m1, m2;
    cin >> m1 >> m2;
    auto m1m2 = m1 * m2;
    cout << m1m2 << endl;
    /*scalef4 sa;
    scalef4 sb;
    cin >> sa >> sb;
    cout << sa * sb << endl;
    cout << sa.inverse() << endl;
    xsimd::batch<float, 4> test;
    xsimd::batch<float, 4> d(test);*/
    return 0;
}
