#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    int r[m], t[n+1];
    for(int i = 0; i < m; i++) cin >> r[i];
    t[0] = 0;
    for(int i = 1; i <= n; i++){
        t[i] = n+1;
        for(int y = 0; y < m; y++)
            if(i-r[y] >= 0)
                t[i] = min(t[i], t[i-r[y]]+1);
    }
    if(t[n] >= n+1) cout << "NIE";
    else cout << t[n];
}
