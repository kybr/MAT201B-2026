#include "al/app/al_App.hpp"  // al::App
#include "al/math/al_Random.hpp"

using namespace al;
using namespace std;


double rs() {
    return rnd::uniformS(1.0);
}
int main() {
    Quatd a(Vec3d(rs(), rs(), rs()));
    cout << a.w << endl;
    cout << a.x << endl;
    cout << a.y << endl;
    cout << a.z << endl;

    a.normalize();

    cout << a.mag() << endl;

    cout << sqrt(
        a.w * a.w + 
        a.x * a.x + 
        a.y * a.y + 
        a.z * a.z 
    ) << endl;
}