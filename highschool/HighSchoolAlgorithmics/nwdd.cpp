#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin >> n;
    long long t[n], k[n], /*w, m,*/ nwd;
    for (int i=0; i<n; i++){
        cin >> t[i] >> k[i];
       // w=max(t[i], k[i]);
       // m=min(t[i], k[i]);
        nwd = __gcd(t[i], k[i]);
        t[i]/=nwd;
        k[i]/=nwd;
        cout << t[i] << ' ' << k[i] << endl;
    }
}
