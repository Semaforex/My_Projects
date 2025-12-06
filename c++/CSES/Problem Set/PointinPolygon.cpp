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

bool On(point a, point b, point c){
    if(dot(a, b, c) != 0) return false;
    if(min(a.x, b.x) <= c.x && max(a.x, b.x) >= c.x && min(a.y, b.y) <= c.y && max(a.y, b.y) >= c.y) return true;
    return false;
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

bool intersect(point a, point b, point c, point d){
    if((a.x-b.x)*(c.y-d.y) == (c.x-d.x)*(a.y-b.y) && !on(a, b, c, d))
        return false;
    if(dot(a, b, c)*dot(a, b, d) <= 0 && dot(c, d, a)*dot(c, d, b) <= 0)
        return true;
    return false;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    point p[n+1], P;
    for(int i = 0; i < n; i++)
        cin >> p[i].x >> p[i].y;
    p[n] = p[0];
    for(int i = 0; i < m; i++){
        bool b = false;
        cin >> P.x >> P.y;
        for(int y = 0; y < n; y++){
            if(On(p[y], p[y+1], P)){
                cout << "BOUNDARY" << '\n';
                b = true;
                break;
            }
        }
        if(b) continue;
        int tot = 0;
        point A;
        A.x = 1000*1000*1000+1;
        A.y = P.y+1;
        for(int y = 0; y < n; y++)
            tot += (intersect(p[y], p[y+1], P, A));
        if(tot%2) cout << "INSIDE" << '\n';
        else cout << "OUTSIDE" << '\n';
    }
}
