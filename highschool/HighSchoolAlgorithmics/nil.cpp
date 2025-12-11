#include <bits//stdc++.h>
using namespace std;
vector<int> g[1000001];
int odw[1000001];
long long wart[1000001], m;
void DFS(int ob,long long w){
    odw[ob] = 1;
    w = wart[ob] + w;
    m = max(m, w);
    for(int i = 0; i < g[ob].size(); i++){
        if(!odw[g[ob][i]])
            DFS(g[ob][i], w);
    }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, a, b;
    cin >> n;
    for(int i = 1; i <= n; i++){
        cin >> a;
        wart[i] = a;
    }
    for(int i = 1; i < n; i++){
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    DFS(1, 0);
    cout << m;
}
