#include <bits//stdc++.h>
using namespace std;
vector<int> g[1000001];
int pre[1000001], od[1000001], n;
void DFS(int ob){
    pre[ob] = n;
    n++;
    for(int i = 0; i < g[ob].size(); i++)
        DFS(g[ob][i]);
    n++;
    od[ob] = n;
}
int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int n, a;
    cin >> n;
    for(int i = 1; i < n; i++){
        cin >> a;
        g[a].push_back(i);
    }
    DFS(0);
    while(1){
        cin >> a;
        if(a == -1) return 0;
        cin >> n;
        if(pre[n] > pre[a] && od[a] > od[n]) cout << "TAK" << '\n';
        else cout << "NIE" << '\n';
     }
}
