#include<bits//stdc++.h>
using namespace std;
int odl[101001];
bool odw[101001];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, k, m, a;
    cin >> n >> k >> m;
    vector<int> g[n+m+1];
    for(int i = 1; i <= m; i++){
        for(int y = 0; y < k; y++){
            cin >> a;
            g[a].push_back(n+i);
            g[n+i].push_back(a);
        }
    }
    queue<int> q;
    q.push(1);
    odw[1] = true;
    while(!q.empty()){
        int ob = q.front();
        q.pop();
        for(int i = 0; i < g[ob].size(); i++){
            if(!odw[g[ob][i]]){
                odl[g[ob][i]] = odl[ob]+1;
                odw[g[ob][i]] = true;
                q.push(g[ob][i]);
            }
        }
    }
    if(odl[n] > 0) cout << odl[n]/2;
    else cout << -1;
}
