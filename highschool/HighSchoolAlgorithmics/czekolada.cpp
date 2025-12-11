#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, m, obk = 1, obw = 1, w = 0;
    cin >> n >> m;
    pair<int, int> t[n+m-2];
    for(int i = 0; i < n-1; i++){
        cin >> t[i].first;
        t[i].second = 1;
    }
    for(int i = 0; i < m-1; i++){
        cin >> t[i+n-1].first;
        t[i+n-1].second = 0;
    }
    sort(t, t+n+m-2);
    for(int i = n+m-3; i >= 0; i--){
        if(t[i].second == 1){
            w+= t[i].first*obk;
            obw++;
        }
        else{
            w+= t[i].first*obw;
            obk++;
        }
    }
    cout << w;
}
