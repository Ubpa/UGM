#include <iostream>

#include <UGM/vec.hpp>
#include <UGM/point.hpp>
#include <UGM/mat.hpp>

using namespace Ubpa;
using namespace std;

int main(int argc, char* argv[])
{
    //{// vecf4
    //    vecf4 a;
    //    vecf4 b;
    //    cin >> a >> b;
    //    cout << a + b << endl;
    //    cout << a.dot(b) << endl;
    //}

    //{// pointf4
    //    pointf4 p0, p1;
    //    cin >> p0 >> p1;
    //    p0 -= vecf4{ 1,1,1,1 };
    //    cout << p0 - p1 << endl;
    //}
    
    //{ // matrix mul
    //    matf4 m1, m2;
    //    cin >> m1 >> m2;
    //    auto m1m2 = m1 * m2;
    //    cout << m1m2 << endl;
    //}

    //{// matrix inverse
    //    matf4 m;
    //    cin >> m;
    //    (m * m.inverse()).print();
    //}

    //{ // cast and []
    //    {
    //        vecf4 v;
    //        cin >> v;
    //        pointf3 p = v.cast_to<pointf3>();
    //        cout << p << endl;
    //    }

    //    {
    //        vecf4 v;
    //        cin >> v;
    //        pointf3 q{ v[0], v[1], v[2] };
    //        cout << q << endl;
    //    }
    //}

    //{ // min
    //    vecf4 s;
    //    cin >> s;
    //    const vecf4 ss = s;
    //    auto rst = ss.min_component();
    //    cout << rst << endl;
    //}

    //{
    //    linef3 l;
    //    cin >> l;
    //    bboxf3 b0, b1;
    //    cin >> b0 >> b1;

    //    // 36 and have loop
    //    //auto [success, t0, t1] = l.intersect(b);

    //    // 33 and have loop without simd
    //    auto invdir = l.inv_dir();
    //    auto [success0, t00, t01] = l.intersect(b0, invdir);
    //    cout << success0 << t00 << t01 << endl;

    //    // 26
    //    auto [success1, t10, t11] = l.intersect(b1, invdir);
    //    cout << success1 << t10 << t11 << endl;
    //}

    /*{
        vecf4 u, v;
        cin >> u >> v;
        cout << vecf4::v3_dot(u, v) << endl;
        cout << vecf4::v3_cross(u, v) << endl;
    }*/

    //{
    //    linef3 l;
    //    trianglef3 tri;
    //    cin >> l >> tri;

    //    l.print();
    //    cout << tri << endl;

    //    // 103 -> 58
    //    auto [s, uwv, t] = l.intersect(tri);
    //    cout << s << endl;
    //    cout << uwv[0] << ", " << uwv[1] << ", " << uwv[2] << endl;
    //    cout << t << endl;
    //}

    //{
    //    pointf3 p;
    //    vecf3 d;
    //    cin >> p >> d;
    //    rayf3 r{ p,d };

    //    auto [isIntersect0, t0, uv] = r.intersect_std_disk();
    //    cout << isIntersect0 << ", " << t0 << ", " << uv[0] << ", " << uv[1] << ", " << endl;

    //    // 51 -> 36
    //    auto [isIntersect, t] = r.intersect_std_sphere();
    //    cout << isIntersect << endl << t << endl;
    //}

    {
        matf4 m;
        cin >> m;
        auto tm = m.transpose();
        cout << tm << endl;
        auto m2 = m + m;
        cout << m2 << endl;
    }

    return 0;
}
