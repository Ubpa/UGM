#include <iostream>
#include <UGM/vec.h>
#include <UGM/point.h>
#include <UGM/mat.h>
//#include <UGM/scale.h>

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

    {// matrix inverse
        matf4 m;
        cin >> m;
        (m * m.inverse()).print();
    }

    return 0;
}
