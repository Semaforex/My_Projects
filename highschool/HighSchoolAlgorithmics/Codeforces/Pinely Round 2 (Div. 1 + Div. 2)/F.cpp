#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    for(int p = 0; p < t; p++){
        int n;
        cin >> n;
        int t[n+2], pref[n+2], suf[n+2];
        t[0] = 0;
        t[n+1] = 0;
        pref[0] = 0;
        suf[n+1] = 0;
        for(int i = 1; i <= n; i++){
            cin >> t[i];
            pref[i] = pref[i-1]^t[i];
        }
        for(int i = n; i > 0; i--)
            suf[i] = suf[i+1]^t[i];

    }
}
