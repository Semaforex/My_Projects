#include <bits//stdc++.h>
using namespace std;
int odw[100001];
vector<int> g[100001];
void DFS(int a){
    odw[a] = 1;
    for(int i = 0; i < g[a].size(); i++)
        if(!odw[g[a][i]]) DFS(g[a][i]);
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, x, y;
    cin >> n >> m;
    for(int i = 0; i < m; i++){
        cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }
    DFS(1);
    for(int i = 1; i <= n; i++){
        if(odw[i]) cout << "TAK" << '\n';
        else cout << "NIE" << '\n';
    }
}
