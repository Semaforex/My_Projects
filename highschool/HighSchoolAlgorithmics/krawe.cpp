#include <iostream>
using namespace std;
int t[1001][1001];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, k, l;
    cin >> n >> m;
    for(int i = 0; i < m; i++){
        cin >> l >> k;
        t[l][k] = 1;
    }
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> l >> k;
        if(t[l][k] || t[k][l]) cout << "TAK" << '\n';
        else cout << "NIE" << '\n';
    }
}
