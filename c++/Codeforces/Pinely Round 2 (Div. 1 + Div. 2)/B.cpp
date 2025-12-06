#include<iostream>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    for(int p = 0; p < t; p++){
        int n, x, s = 0;
        cin >> n;
        int t[n+1];
        for(int i = 0; i <= n; i++)
            t[i] = 0;
        for(int i = 0; i < n; i++){
            cin >> x;
            t[x]++;
            if(t[x-1]) t[x-1]--;
        }
        for(int i = 1; i <= n; i++)
            s += t[i];
        cout << s-1 << '\n';
    }
}
