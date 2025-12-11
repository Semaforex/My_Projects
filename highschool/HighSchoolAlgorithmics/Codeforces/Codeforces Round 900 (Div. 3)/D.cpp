#include<bits//stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int Q = 0; Q < T; Q++){
        int n, k, q, x;
        cin >> n >> k;
        char s[n];
        int l[k], r[k], pos[n+1];
        for(int i = 0; i < n; i++)
            cin >> s[i];
        for(int i = 0; i < k; i++)
            cin >> l[i];
        for(int i = 0; i < k; i++){
            cin >> r[i];
            for(int y = l[i]; y <= r[i]; y++)
                pos[y] = i;
        }
        cin >> q;
        for(int i = 0; i < q; i++){
            cin >> x;
            int a = r[pos[x]]+l[pos[x]]-x;
            if(a > x) swap(a, x);
            reverse(s+a-1, s+x);
        }
        for(int i = 0; i < n; i++) cout << s[i];
        cout << '\n';
    }
}
