#include<iostream>
using namespace std;

struct point{
    long long x;
    long long y;
};

long long dot(point a, point b, point c){
    long long d = (c.x-a.x)*(b.y-a.y)-(c.y-a.y)*(b.x-a.x);
    if(d < 0) return -1;
    if (d > 0) return 1;
    return 0;
}

bool on(point a, point b, point c, point d){
    int mx = min(a.x, b.x), Mx = max(a.x, b.x);
    int my = min(a.y, b.y), My = max(a.y, b.y);
    if(mx <= c.x && my <= c.y && Mx >= c.x && My >= c.y) return true;
    if(mx <= d.x && my <= d.y && Mx >= d.x && My >= d.y) return true;
    mx = min(c.x, d.x), Mx = max(c.x, d.x);
    my = min(c.y, d.y), My = max(c.y, d.y);
    if(mx <= a.x && my <= a.y && Mx >= a.x && My >= a.y) return true;
    if(mx <= b.x && my <= b.y && Mx >= b.x && My >= b.y) return true;
    return false;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int Q = 0; Q < T; Q++){
        point a, b, c, d;
        cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y >> d.x >> d.y;

        if((a.x-b.x)*(c.y-d.y) == (c.x-d.x)*(a.y-b.y) && !on(a, b, c, d)){
            cout << "NO" << '\n';
            continue;
        }
        if(dot(a, b, c)*dot(a, b, d) <= 0 && dot(c, d, a)*dot(c, d, b) <= 0) cout << "YES" << '\n';
        else cout << "NO" << '\n';
    }
}
