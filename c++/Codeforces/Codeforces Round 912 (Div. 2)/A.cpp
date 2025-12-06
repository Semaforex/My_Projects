#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int Q = 0; Q < T; Q++){
        int n, k;
        cin >> n >> k;
        int t[n];
        for(int i = 0; i < n; i++)
            cin >> t[i];
        if(k >= 2)
            cout << "YES" << '\n';
        else{
            bool issorted = true;
            for(int i = 1; i < n; i++){
                if(t[i] < t[i-1]) issorted = false;
            }
            if(issorted) cout << "YES" << '\n';
            else cout << "NO" << '\n';
        }
    }
}
