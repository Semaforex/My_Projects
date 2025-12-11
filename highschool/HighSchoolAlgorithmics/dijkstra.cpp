#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, a, b, c;
    cin >> n >> m;
    vector<pair<int, int>> g[n+1];
    for(int i = 0; i < m; i++){
        cin >> a >> b >> c;
        g[a].push_back(make_pair(b, c));
        g[b].push_back(make_pair(a, c));
    }
    long long odl[n+1];
    for(int i = 2; i <= n; i++) odl[i] = -1;
    odl[1] = 0;
    priority_queue<pair<long long, int>> q;
    q.push(make_pair(0, 1));
    while(!q.empty()){
        int ob = q.top().second;
        long long d = -q.top().first;
        q.pop();
        for(int i = 0; i < g[ob].size(); i++){
            int sas = g[ob][i].first, k = g[ob][i].second;
            if(odl[sas] > odl[ob] + k || odl[sas] == -1){
                odl[sas] = k + odl[ob];
                q.push(make_pair(-odl[sas], sas));
            }
        }
    }
    for(int i = 1; i <= n; i++)
        cout << odl[i] << '\n';
}
