#include <bits//stdc++.h>
using namespace std;
struct point{
    long long x;
    long long y;
};
long long w(point a, point b, point c){
    return a.x*b.y+b.x*c.y+c.x*a.y-b.x*a.y-b.y*c.x-c.y*a.x;
}
int cross(point a, point b, point c, point d){
    if(w(a, c, d) == 0 && a.x <= max(c.x, d.x) && a.x >= min(c.x, d.x) && a.y >= min(c.y, d.y) && a.y <= max(c.y, d.y)) return 2; //jeœli punkt A le¿y na odc CD
    if(w(a, b, c)*w(b, a, d) > 0 && w(c, d, a)*w(d, c, b) > 0) return 1;
    return 0;
}
int main(){
    int n, w;
    cin >> n;
    point t[n], p;
    for(int i = 0; i < n; i++)
       cin >> t[i].x >> t[i].y;
    cin >> p.x >> p.y;
    point q;
    q.x = 1000001;
    q.y = p.y+1;
    for(int i = 0; i < n-1; i++){
        if(cross(p, q, t[i], t[i+1])) w++;
        if(cross(p, q, t[i], t[i+1]) == 2){
            cout << "NA KRAWEDZI";
            return 0;
        }
    }
    if(cross(p, q, t[n-1], t[0])) w++;
    if(cross(p, q, t[n-1], t[0]) == 2){
        cout << "NA KRAWEDZI";
        return 0;
    }
    if(w%2) cout << "TAK";
    else cout << "NIE";

}
