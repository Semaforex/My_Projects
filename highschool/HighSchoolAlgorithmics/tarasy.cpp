#include <bits//stdc++.h>
using namespace std;
int n, k;
int gasienica(int t[]){
    int p = 0, w = 1, ob = 0, najl = 1;
    while(ob+(abs(t[p+1]-t[p])+t[p+1]-t[p])/2 <= k && p < n-1){
        ob += (abs(t[p+1]-t[p])+t[p+1]-t[p])/2;
        p++;
        w++;
    }
    najl = max(najl, w);
    while(t[k+1] <= t[k] && k < p){
        k++;
        w--;
    }
    if(t[k+1] > t[k]){
        ob -= t[k+1]-t[k];
        w--;
    }
    return najl;
}
int main(){
    cin >> n >> k;
    int t[n], odt[n], x;
    for(int i = 0; i < n; i++){
        cin >> x;
        t[i] = x;
        odt[n-1-i] = x;
    }
    cout << max(gasienica(t), gasienica(odt));
}
