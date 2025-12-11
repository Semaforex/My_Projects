#include <bits//stdc++.h>
using namespace std;
int k[300001], n;
int bin_Search(int p, int ko, int szuk){
    int sr = (p+ko)/2;
    if(sr == 0) return 0;
    if(sr == n-1 || (k[sr] >= szuk && k[sr+1] < szuk)) return sr;
    if(k[sr] < szuk) return bin_Search(p, sr-1, szuk);
    return bin_Search(sr+1, ko, szuk);

}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int m, x, w = 0;
    cin >> n >> m;
    cin >> k[0];
    for(int i = 1; i < n; i++){
        cin >> k[i];
        if(k[i] > k[i-1]) k[i] = k[i-1];
    }
    for(int i = 0; i < m; i++){
        cin >> x;
        w = bin_Search(0, n-1, x);
        n = w;
    }
    if(w == 0) cout << w;
    else cout << w+1;
}
