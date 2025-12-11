#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, ob;
    cin >> n >> m;
    int r[m];
    pair<int, vector<int>> t[n+1];
    for(int i = 0; i < m; i++) cin >> r[i];
    t[0].first = 0;
    for(int i = 1; i <= n; i++){
        t[i].first = n+1;
        for(int y = 0; y < m; y++)
            if(i-r[y] >= 0)
                if(t[i].first > t[i-r[y]].first+1){
                    t[i].first = t[i-r[y]].first+1;
                    ob = r[y];
                }
        if(t[i].first != n+1){
            t[i].second.push_back(ob);
            for(int i = 0; i < t[i-ob].second.size(); i++) t[i].second.push_back(t[i-ob].second[i]);
        }
    }
    if(t[n].first >= n+1) cout << "NIE";
    else{
        cout << t[n].first << endl;
        for(int i = 0; i < t[n].second.size(); i++) cout << t[n].second[i] << " ";
    }
}
