#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, m, a, b, ma;
    cin >> n >> m;
    int t[n];
    int pref[n+1];
    pref[0]=0;
    for (int i = 0; i < n; i++) cin >> t[i];
    for (int i = 1; i <= n; i++) pref[i] = pref[i-1] + t[i-1];
    for (int i = 0; i < m; i++){
        cin >> a >> b;
        ma = -1;
        for (int j = a-1; j < b; j++){
            ma = max(ma, t[j]);
        }
        if(2*ma<pref[b]-pref[a-1]) cout << "TAK" << endl;
        else cout << "NIE" << endl;

    }
}
