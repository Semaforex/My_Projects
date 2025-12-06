#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    int t[n], w[m];
    pair<int, int> o[m];
    for(int i = 0; i < n; i++) cin >> t[i];
    for(int i = 0; i < m; i++){
        cin >> o[i].first;
        o[i].second = i;
    }
    sort(o, o+m);
    int ob = 0, najw = 0;
    while(ob < m){
        while(t[najw] < o[ob].first && najw < n) najw++;
        w[o[ob].second] = najw;
        ob++;
    }
    for(int i = 0; i < m; i++) cout << w[i] << ' ';
}
