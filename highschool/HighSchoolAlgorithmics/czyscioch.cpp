#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, q, x, p, v, e;
    cin >> n;
    int t[n+1];
    for (int i=1; i<=n; i++){
        cin >> t[i];
    }
    cin >> q;
    for (int i=0; i<q; i++){
        cin >> x;
        if(x == 1){
            cin >> p >> v;
            t[p] = v;
        }
        else{
            if(x == 2){
                cin >> p >> e >> v;
                for (int y = 1; y <= n; y++)
                    if(t[y] >= p && t[y] <= e) t[y] = v;
            }
            else{
                cin >> p;
                cout << t[p] << endl;
            }
        }
    }
}
