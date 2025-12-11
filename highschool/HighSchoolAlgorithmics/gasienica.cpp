#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, m;
    cin >> n >> m;
    int t[n], w = 1000001;
    for(int i = 0; i < n; i++) cin >> t[i];
    int p = -1, k = -1, obr = 0, obk = 0;
    while(p < n){
        while(obr < m && p < n-1){
            p++;
            obr += t[p];
            obk -= t[p]-1;
        }
        if(obr < m){
            if(w == 1000001) cout << "NIE";
            else cout << w;
            return 0;
        }
        w = min(w, obk);
        int kap = obk;
        while(obk == kap && k < p){
            k++;
            obr -= t[k];
            obk += t[k]-1;
        }
    }
    cout << w;
}
