#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    int t[n+1], x = 0;
    t[0] = 0;
    for (int i = 1; i <= n; i++){
        cin >> t[i];
        if(t[i]+m*x < t[i-1]) x++;
        t[i] = t[i] + m*x;
    }
    long long pocz = 1, kon = 1, obwyn = 1, obczas = 1, wyn = 0;
    while (pocz < n){
        if(obczas + t[pocz+1]-t[pocz] <= m){
            obczas += t[pocz+1]-t[pocz];
            obwyn ++;
            pocz++;
        }
        else{
            obwyn--;
            kon++;
            obczas -= t[kon]-t[kon-1];
        }
        wyn = max(wyn, obwyn);
    }
    cout << wyn;
}
