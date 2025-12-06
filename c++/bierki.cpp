#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, a = 0, b = 1, c = 2, w = 0;
    cin >> n;
    int t[n];
    for(int i = 0; i < n; i++) cin >> t[i];
    sort(t, t+n);
    while(c < n-1){
        while(t[a]+t[b] > t[c+1] && c < n-1)  c++;
        if(t[c] < t[a]+t[b]) w = max(w, c-a+1);
        a++;
        b++;
    }
    cout << w;
}
