#include<iostream>
#include<algorithm>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int q;
    cin >> q;
    for(int z = 0; z < q; z++){
        int n;
        cin >> n;
        int t[n+1], pref[n+1];
        pref[0] = 0;
        string s;
        for(int i = 1; i <= n; i++){
            cin >> t[i];
            pref[i] = pref[i-1]^t[i];
        }
        cin >> s;
        int x1 = 0, x0 = 0;
        for(int i = 0; i < n; i++){
            if(s[i] == '0') x0 ^= t[i+1];
            else x1 ^= t[i+1];
        }
        int p;
        cin >> p;
        for(int i = 0; i < p; i++){
            int a, b, c;
            cin >> a;
            if(a == 1){
                cin >> b >> c;
                int dif = pref[c]^pref[b-1];
                x0 ^= dif;
                x1 ^= dif;
            }
            else{
                cin >> b;
                if(b == 0) cout << x0 << ' ';
                else cout << x1 << ' ';
            }
        }
        cout << '\n';
    }
}
