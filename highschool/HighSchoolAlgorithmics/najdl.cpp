#include <iostream>
using namespace std;
int t[100001], w[100001];
int b_search(int p, int k, int sz){
    int sr = (p+k)/2;
    if(w[sr] < sz && w[sr+1] == -1) return sr+1;
    if(w[sr-1] < sz && w[sr] >= sz) return sr;
    if(w[sr] >= sz) return b_search(p, sr-1, sz);
    if(w[sr] < sz) return b_search(sr+1, k, sz);
}
int main(){
    int n, ob = 1;
    cin >> n;
    for(int i = 2; i <= 100001; i++) w[i] = -1;
    for(int i = 1; i <= n; i++) cin >> t[i];
    w[1] = t[1];
    for(int i = 2; i <= n; i++){
        w[b_search(1, ob+1, t[i])] = t[i];
        if(w[ob+1] != -1) ob++;
    }
    cout << ob;

}
