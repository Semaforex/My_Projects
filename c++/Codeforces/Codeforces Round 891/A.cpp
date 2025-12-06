#include<bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    for(int q = 0; q < t; q++){
        int n, s = 0, x;
        cin >> n;
        for(int i = 1; i < n; i++){
            cin >> x;
            s += x;
        }
        cin >> x;
        if(x%2 == s%2) cout << "YES";
        else cout << "NO";
        cout << '\n';
    }
}
