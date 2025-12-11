#include <bits//stdc++.h>
using namespace std;
int odw[100001], sp[100001];
vector<int> g[100001];
void DFS(int ob, int s){
    odw[ob] = 1;
    sp[ob] = s;
    for(int i = 0; i < g[ob].size(); i++)
        if(!odw[g[ob][i]])
            DFS(g[ob][i], s);
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, a, b;
    cin >> n >> m >> q;
    for(int i = 0; i < m; i++){
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    a = 1;
    while(a <= n){
        if(!odw[a])
            DFS(a, a);
        a++;
    }
    for(int i = 0; i < q; i++){
        cin >> a >> b;
        if(sp[a] == sp[b])
            cout << 'T' << '\n';
        else
            cout << 'N' << '\n';
    }
}
