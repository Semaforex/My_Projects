#include <bits//stdc++.h>
using namespace std;
int t[51][51], k, n, m, sr = 125000, lewo, prawo, rozm = 250000;
int wp[250001], wo[250001];
void sufix(){
    cin >> k >> n >> m;
    for(int i = 1; i <= n; i++)
        for(int y = 1; y <= k; y++){
            cin >> t[i][y];
            t[i][y] += t[i][y-1];
            //cout << t[i][y] << ' ';
        }
    //cout << '\n';
    for(int i = n+1; i <= n+m; i++)
        for(int y = 1; y <= k; y++){
            cin >> t[i][y];
            t[i][y] *= -1;
            t[i][y] += t[i][y-1];
            //cout << t[i][y] << ' ';
        }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    sufix();
    lewo = sr;
    prawo = sr;
    n += m;
    for(int y = 0; y <= rozm; y++){
        wp[y] = -1;
        wo[y] = -1;
    }
    for(int i = 1; i <= n; i++){
        for(int y = lewo; y <= prawo; y++)
            wo[y] = wp[y];
        for(int y = 1; y <= k; y++){
            wo[t[i][y]+sr] = max(wo[t[i][y]+sr], y);
            if(t[i][y]-t[i][y-1] < 0)
                lewo = max(0, lewo+t[i][y]-t[i][y-1]);
            else
                prawo = min(rozm, prawo+t[i][y]-t[i][y-1]);
            cout << lewo << ' ' << prawo << '\n';
            for(int z = lewo; z <= prawo; z++)
                if(z-t[i][y] >= 0 && z+t[i][y] <= rozm)
                    if(wp[z-t[i][y]] != -1)
                        wo[z] = max(wo[z], wp[z-t[i][y]]+y);
            }
        for(int y = lewo; y <= prawo; y++)
            wp[y] = wo[y];
    }
    if(wp[sr] != -1)
        cout << k*n-wp[sr];
    else cout << k*n;
}
