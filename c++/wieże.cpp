#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    int t[n];
    for (int i=0; i<n; i++){
        cin >> t[i];
    }
    int pocz = t[0];
    sort(t, t+n);
    int maxim = max(pocz-t[0], t[n-1]-pocz);
    int minim = min(pocz-t[0], t[n-1]-pocz);
    cout << 2*(minim)+maxim;
}
