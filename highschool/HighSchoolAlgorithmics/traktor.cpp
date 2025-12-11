#include <bits//stdc++.h>
using namespace std;
int z[1100001], odp, s[100001];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, k, r, a, x, w;
    cin >> n >> k >> r;
    queue<int> q;
    vector<int> g[n+r+1];
    for(int i = 0; i < k; i++){
        cin >> a;
        z[a]++;
        q.push(a);
    }
    for(int i = 1; i <= r; i++){
        cin >> s[i];
        for(int y = 0; y < s[i]; y++){
            cin >> w;
            g[w].push_back(n+i);
        }
        cin >> x;
        for(int y = 0; y < x; y++){
            cin >> w;
            g[n+i].push_back(w);
        }
    }
    while(!q.empty()){
        int ob = q.front();
        q.pop();
        if(ob > n){
            for(int i = 0; i < g[ob].size(); i++){
                int pr = g[ob][i];
                if(!z[pr]){
                    z[pr]++;
                    q.push(pr);
                    odp++;
                }
            }
        }
        else{
            for(int i = 0; i < g[ob].size(); i++){
                int re = g[ob][i];
                z[re]++;
                if(z[re] == s[re-n])
                    q.push(re);
            }
        }
    }
    cout << odp;
}
