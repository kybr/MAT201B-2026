#include "al/app/al_App.hpp"  // al::App

using namespace al;
using namespace std;

int main() {
    Vec3f a{1, 1, 0};
    Vec3f b = a * 0.1;
    cout << a << endl; // << is the "stream" operator; 
    cout << b << endl;

    int m;
    printf("enter a number: "); // prompt
    cin >> m;
    printf("%d\n", m);

    Nav h;
}