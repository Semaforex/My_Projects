#include<bits//stdc++.h>
using namespace std;
float gap[41], minim[41];
float gdzie(float a, float x){
    int cur;
    for(int i = 0; i < 41; i++)
        if(abs(a) > minim[i])
            cur = i;
    if(abs(x) <= gap[cur]) return a;
    if(a*x >= 0){
        while(abs(x) > gap[cur])
            cur++;
        if (x < 0) return -minim[cur];
        return minim[cur];
    }
    if(x < 0){
        x *= -1;
        while(x > gap[cur])
            cur++;
        return -minim[cur];
    }
    while(x > gap[cur])
        cur++;
    return minim[cur];
}
int main(){
    float x, y, z, a, b, c;
    minim[0] = 0;
    minim[1] = 1;
    gap[40] = 16384*2;
    for(int i = 39; i >= 0; i--)
        gap[i] = gap[i+1]/2;
    for(int i = 2; i < 41; i++)
        minim[i] = minim[i-1]*2;
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> a >> b >> c;
        cin >> x >> y >> z;
        cout << setprecision(4) << fixed << gdzie(a, x) << ' ' << gdzie(b, y) << ' ' << gdzie(c, z) << '\n';
    }
}
