#include <iostream>
#include <vector>
using namespace std;
int odw[100001], q;
bool p;
vector<int> g[100001];
void DFS(int ob){
    q++;
    odw[ob] = 1;
    int w = 0;
    for(int i = 0; i < g[ob].size(); i++){
        if(odw[g[ob][i]]) w++;
        else DFS(g[ob][i]);
    }
    if(w > 1){
        p = 1;
    }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, a, b;
    cin >> n >> m;
    for(int i = 0; i < m; i++){
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    DFS(1);
    if(p || q < n) cout << "NIE";
    else cout << "TAK";
}
