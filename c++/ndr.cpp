#include <bits//stdc++.h>
using namespace std;
int odw[100001];
int main(){
    int n, m, a, b, w;
    cin >> n >> m;
    vector<pair<int, int>> g[n+1];
    vector<pair<int, int>> MDR[n+1];
    for(int i = 0; i < m; i++){
        cin >> a >> b >> w;
        g[a].push_back(make_pair(b, w));
        g[b].push_back(make_pair(a, w));
    }
    priority_queue<tuple<int, int, int>> q;
    for(int i = 0; i < g[1].size(); i++)
        q.push(make_tuple(-g[1][i].second, 1, g[1][i].first));
    odw[1] = 1;
    while(!q.empty()){
        pair<int, int> k = make_pair(get<1>(q.top()), get<2>(q.top()));
        int w = -get<0>(q.top());
        q.pop();
        if(!odw[k.second]){
            for(int i = 0; i < g[k.second].size(); i++)
                q.push(make_tuple(-g[k.second][i].second, k.second, g[k.second][i].first));
            odw[k.second] = 1;
            MDR[k.first].push_back(make_pair(k.second, w));
            MDR[k.second].push_back(make_pair(k.first, w));
        }
    }
    for(int i = 1; i <= n; i++){
        cout << "krawedzie z wierzcholka " << i << ": ";
        for(int y = 0; y < MDR[i].size(); y++){
             cout << '(' << i << ", " << MDR[i][y].first << ") ";
        }
        cout << '\n';
    }
}
