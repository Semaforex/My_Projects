#include <bits//stdc++.h>
using namespace std;
int odl[100001];
vector<int> g[100001];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, a, b;
    cin >> n >> m;
    for(int i = 2; i <= n; i++)
        odl[i] = -1;
    for(int i = 0; i < m; i++){
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    odl[1] = 0;
    queue<int> q;
    q.push(1);
    while(!q.empty()){
        int ob = q.front();
        q.pop();
        for(int i = 0; i < g[ob].size(); i++){
            if(odl[g[ob][i]] == -1){
                odl[g[ob][i]] = odl[ob]+1;
                q.push(g[ob][i]);
            }

        }
    }
    for(int i = 1; i <= n; i++)
        cout << odl[i] << '\n';

}
