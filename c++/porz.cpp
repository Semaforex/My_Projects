#include <bits//stdc++.h>
using namespace std;
int s[1000001];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    priority_queue<int> q;
    int n, m, a, b, ob;
    cin >> n >> m;
    vector<int> g[n+1], ts;
    for(int i = 0; i < m; i++){
        cin >> a >> b;
        g[a].push_back(b);
        s[b]++;
    }
    for(int i = 1; i <= n; i++)
        if(!s[i]) q.push(-i);
    while(!q.empty()){
        ob = -q.top();
        q.pop();
        ts.push_back(ob);
        for(int i = 0; i < g[ob].size(); i++){
            s[g[ob][i]]--;
            if(!s[g[ob][i]]) q.push(-g[ob][i]);
        }
    }
    if(ts.size() == n){
        cout << "TAK" << '\n';
        for(int i = 0; i < n; i++)
            cout << ts[i] << ' ';
    }
    else cout << "NIE";
}
