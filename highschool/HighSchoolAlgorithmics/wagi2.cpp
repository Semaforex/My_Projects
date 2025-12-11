#include <bits//stdc++.h>
using namespace std;
int t[51][51], k, n, m, sr = 125000, lewo, prawo, rozm = 250000, suma = 0;
int wp[250001], wo[250001];
void sufix(){
    cin >> k >> n >> m;
    for(int i = 1; i <= n; i++)
        for(int y = 1; y <= k; y++)
            cin >> t[i][y];
    for(int i = n+1; i <= n+m; i++)
        for(int y = 1; y <= k; y++){
            cin >> t[i][y];
            t[i][y] *= -1;
        }
    for(int i = 1; i <= n+m; i++)
        for(int y = k-1; y >= 1; y--){
            t[i][y] += t[i][y+1];
            suma += t[i][y];
        }
}
int main(){
    sufix();
    lewo = sr;
    prawo = sr;
    n += m;
    for(int i = 1; i <= rozm; i++)
        wp[i] = rozm+1;
    for(int i = 1; i <= n; i++){
        for(int y = 0; y <= rozm; y++)
            wo[y] = wp[y];
        for(int y = 1; y <= k; y++){
            wo[t[i][y]+sr] = min(wo[t[i][y]+sr], y);
            lewo = max(0, min(lewo, lewo - abs(t[i][y])));
            prawo = min(rozm, max(prawo, prawo + abs(t[i][y])));
            for(int z = lewo; z <= prawo; z++)
                if(z+t[i][y] >= 0 && z+t[i][y] <= rozm)
                    wo[z] = min(wo[z], wp[z+t[i][y]]+y);
            }
        for(int y = 0; y <= rozm; y++)
            wp[y] = wo[y];
    }
    if(wp[sr+suma] != rozm+1)
        cout << wp[sr+suma];
    else
        cout << k*n;
}
