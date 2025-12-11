#include<iostream>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int q, n;
    cin >> q;
    for(int x = 0; x < q; x++){
        cin >> n;
        int w=0, m[n+1], ob;
        m[0] = 0;
        for(int i = 1; i <= n; i++){
            cin >> ob;
            m[i] = max(m[i-1], ob);
            if(m[i] != ob) w = max(w, m[i]);
        }
        cout << w << '\n';
    }
}
