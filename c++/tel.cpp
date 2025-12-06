#include <bits//stdc++.h>
using namespace std;
bool com(int a, int b){
    if(a/1000 == b/1000){
        if(a%10 == b%10){
            if(a%100/10 == b%100/10) return a < b;
            return a%100/10 < b%100/10;
        }
        return a%10 < b%10;
    }
    return a/1000 < b/1000;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    int t[n];
    for (int i = 0; i < n; i++) cin >> t[i];
    sort(t, t+n, com);
    for (int i = 0; i < n; i++) cout << t[i] << ' ';
}
