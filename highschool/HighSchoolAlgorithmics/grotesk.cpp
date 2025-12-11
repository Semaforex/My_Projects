#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, k;
    cin >> n >> k;
    string s;
    char t[n];
    for (int i = 0;i < n; i++) cin >> t[i];
    for (int i = 0;i < n/2; i++){
        if(t[i] != t[n-1-i]){
            t[i] = t[n-1-i];
            k--;
        }
    }
    if(k < 0) cout << "NIE";
    else{
        cout << "TAK" << endl;
        for (int i = 0;i < n; i++) cout << t[i];
    }
}
