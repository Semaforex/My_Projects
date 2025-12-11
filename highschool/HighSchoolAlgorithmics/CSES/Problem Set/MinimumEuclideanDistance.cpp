#include<bits//stdc++.h>
using namespace std;

struct point{
    long long x;
    long long y;
};

point p[2*100000];
long long llmax = 8*1000000*1000000*1000000;

bool sortx(point a, point b){
    return a.x < b.x;
}
bool sorty(point a, point b){
    return a.y < b.y;
}

long long euc(point a, point b){
    return pow((a.x-b.x), 2)+pow((a.y-b.y), 2);
}

long long shortest(int a, int b){
    if(a == b) return llmax;
    if(b-a == 1) return euc(p[a], p[b]);
    int mid = (a+b)/2;
    long long d = min(shortest(a, mid), shortest(a+1, b));
    vector<point> strip;
    for(int i = a; i <= b; i++)
        if(p[i].x-mid < d) strip.push_back(p[i]);
    sort(strip.begin(), strip.end(), sorty);
    for(int i = 1; i < strip.size(); i++)
        d = min(d, euc(p[i-1], p[i]));
    return d;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    long long d[n];
    for(int i = 0; i < n; i++)
        cin >> p[i].x >> p[i].y;
    sort(p, p+n, sortx);
    cout << shortest(0, n-1);
}
