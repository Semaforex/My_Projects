#include <bits//stdc++.h>
using namespace std;
int D[501][1002];
int main(){
    int n, sr, sm=0;
    cin >> n >> sr;
    sr*=2;
    int t[n+1];
    for(int i = 1; i <= n; i++){
        cin >> t[i];
        sm+=t[i];
}
    if(sr>sm){
        cout << "NIE";
        return 0;
    }
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= sr; j++){
            D[i][j] = D[i-1][j];
            if(t[i] <= j)
                if(D[i-1][j-t[i]]+t[i] <= j)
                    D[i][j] = max(D[i][j], D[i-1][j-t[i]]+t[i]);
        }
    }
    if(D[n][sr] != sr){
        cout << "NIE";
        return 0;
    }
    cout << "TAK";
    vector<int> v;
    while(sr > 0){
        if(D[n][sr] != D[n-1][sr]){
            v.push_back(t[n]);
            sr -= t[n];
            n--;
        }
        else
            n--;
    }
    cout << '\n' << v.size() << '\n';
    for(int i = 0; i < v.size(); i++) cout << v[i] << ' ';
}
