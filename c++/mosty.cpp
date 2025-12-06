#include <bits//stdc++.h>
using namespace std;
vector<pair<int, int>> odp;
vector<int> g[100001];
int low[100001], num[100001], ojc[100001], r = 1;
bool odw[100001];
void DFS(int ob){
    odw[ob]++;
    num[ob] = r;
    r++;
    for(int i = 0; i < g[ob].size(); i++){
        int sus = g[ob][i];
        if(!odw[sus]){
            ojc[sus] = ob;
            DFS(sus);
        }
    }
    for(int i = 0; i < g[ob].size(); i++){
        int sus = g[ob][i];
        if(sus != ojc[ob]){
            if(num[sus] < num[ob])    //krawedz wtorna
                low[ob] = min(low[ob], num[sus]);
            else                      //syn
                low[ob] = min(low[ob], low[sus]);
        }
    }
    if(low[ob] == ob && g[ob].size() > 1 && ob != 1)
        odp.push_back(make_pair(ob, ojc[ob]));
}
int main(){
    int n, m, a, b;
    cin >> n >> m;
    for(int i = 0; i < m; i++){
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    for(int i = 0; i <= n; i++){
        low[i] = i ;
        num[i] = 1000000;
    }
    DFS(1);
    for(int i = 0; i < odp.size(); i++)
        cout << odp[i].first << ' ' << odp[i].second << '\n';
}
