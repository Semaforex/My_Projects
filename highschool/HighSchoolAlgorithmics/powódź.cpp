#include <bits//stdc++.h>
using namespace std;
vector<int> k[1001];
vector<int> g[1000001];
int lid[1000001], h[1000001], wynik;
bool pompa[1000001];
int lider(int a){
    while(lid[a] != a){
        a = lid[a];
        lid[a] = lid[lid[a]];
    }
    return a;
}
void un(int a, int b){
    a = lider(a);
    b = lider(b);
    if(pompa[b])
        pompa[a] = true;
    lid[b] = a;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, c;
    cin >> n >> m;
    int t[n][m];
    for(int i = 1; i <= n*m; i++) lid[i] = i;
    for(int i = 0; i < n; i++)
        for(int y = 1; y <= m; y++){
            cin >> c;
            h[m*i+y] = abs(c);
            k[abs(c)].push_back((m*i+y)*(c/abs(c)));
        }
    for(int i = 2; i < m; i++){
        g[i].push_back(i-1);
        g[i].push_back(m+i);
        g[i].push_back(i+1);
    }
    for(int i = (n-1)*m+2; i < n*m; i++){
        g[i].push_back(i-1);
        g[i].push_back(i-m);
        g[i].push_back(i+1);
    }
    for(int i = m+1; i < (n-1)*m; i+=m){
        g[i].push_back(i-m);
        g[i].push_back(i+1);
        g[i].push_back(i+m);
    }
    for(int i = 2*m; i < n*m; i+=m){
        g[i].push_back(i-m);
        g[i].push_back(i-1);
        g[i].push_back(i+m);
    }
    g[1].push_back(2);
    g[1].push_back(m+1);
    g[m].push_back(m-1);
    g[m].push_back(2*m);
    g[(n-1)*m+1].push_back((n-2)*m+1);
    g[(n-1)*m+1].push_back((n-1)*m+2);
    g[n*m].push_back(n*m-1);
    g[n*m].push_back((n-1)*m);
    for(int i = 1; i < n-1; i++){
        for(int y = 2; y < m; y++){
            g[m*i+y].push_back(m*i+y+1);
            g[m*i+y].push_back(m*i+y-1);
            g[m*i+y].push_back(m*i+y-m);
            g[m*i+y].push_back(m*i+y+m);
        }
    }
    for(int i = 1; i < 1001; i++){
        for(int y = 0; y < k[i].size(); y++){
            int ob = abs(k[i][y]);
            for(int z = 0; z < g[ob].size(); z++){
                if(h[g[ob][z]] <= h[ob]){
                    un(g[ob][z], ob);
                }
            }
        }
        for(int y = 0; y < k[i].size(); y++){
            int ob = k[i][y];
            if(ob > 0){
                if(!pompa[lider(ob)]){
                    wynik++;
                    pompa[lider(ob)]++;
                }
            }
        }
    }
    cout << wynik;
}

