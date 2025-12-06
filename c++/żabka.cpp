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
    int n, p;
    long long s;
    cin >> n >> p >> s;
    pair<long long, long long> t[n];
    for(int i = 1; i <= n; i++)
        cin >> t[i].first >> t[i].second;
    for(int i = 1; i < n; i++){
        for(int y = i+1; y <=n; y++){
            long long a = (t[i].first-t[y].first)*(t[i].first-t[y].first) + (t[y].second-t[i].second)*(t[y].second-t[i].second);
            if(s*s >= a){
                g[i].push_back(y);
                g[y].push_back(i);
            }
        }
    }
    DFS(p);
    long long d[n+1];
    for(int i = 1; i <= n; i++){
            if(odw[i]) d[i] = ((t[i].first-t[p].first)*(t[i].first-t[p].first)+(t[i].second-t[p].second)*(t[i].second-t[p].second));
            else d[i] = 0;
    }
    sort(d+1, d+n+1);
    double x = sqrt(d[n]), y = s;
    cout << fixed << setprecision(3) << x+y;
}
