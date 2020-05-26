#include <iostream>
#include <UGM/vec.h>

using namespace Ubpa;
using namespace std;

int main(int argc, char* argv[])
{
    vecf4 f0{ 1,2,3,4 };
    vecf4 f1{ 4,3,2,1 };
    cout << f0.is_scalar() << endl;
    cout << vecf4{ 1,1,1,1 }.is_scalar() << endl;
    cout << f0.v3_cos_theta(f1) << endl;
    cout << f0.v3_cross(f1) << endl;
    cout << f0.v3_distance(f1) << endl;
    cout << f0.v3_distance2(f1) << endl;
    cout << f0.v3_dot(f1) << endl;
    cout << f0.v3_is_normalized() << endl;
    cout << f0.v3_norm() << endl;
    cout << f0.v3_norm2() << endl;
    cout << f0.v3_normalize() << endl;
    cout << f0.v3_normalize_self() << endl;
    cout << f0.v3_perpendicular({ 0,1,0,0 }) << endl;
    cout << f0.v3_project({ 0,1,0,0 }) << endl;
    return 0;
}
