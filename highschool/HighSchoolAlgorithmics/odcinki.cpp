#include <iostream>
using namespace std;
int w(point a, point b, point c){
    return a.x*b.y+b.x*c.y+c.x*a.y-b.x*a.y-b.y*c.x-c.y*a.x;
}
bool cross(point a, point b, point c, point d){
    pair<int, int> a, b, c, d;
    cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y >> d.x >> d.y;
    if(w(a, b, c)*w(b, a, d) >= 0 && w(c, d, a)*w(d, c, b) >= 0) return 1;
    return 0;
}
