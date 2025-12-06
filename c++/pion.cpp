#include <iostream>
using namespace std;
int t[10002][10002], odp[10001][10001], wart = 10;
void DFS(int y, int i, int w){
    odp[y][i] = w;
    if(t[y][i] == t[y][i+1] && !odp[y][i+1]) DFS(y, i+1, w);
    if(t[y][i] == t[y+1][i+1] && !odp[y+1][i+1]) DFS(y+1, i+1, w);
    if(t[y][i] == t[y-1][i+1] && !odp[y-1][i+1]) DFS(y-1, i+1, w);
    if(t[y][i] == t[y+1][i] && !odp[y+1][i]) DFS(y+1, i, w);
    if(t[y][i] == t[y-1][i] && !odp[y-1][i]) DFS(y-1, i, w);
    if(t[y][i] == t[y+1][i-1] && !odp[y+1][i-1]) DFS(y+1, i-1, w);
    if(t[y][i] == t[y][i-1] && !odp[y][i-1]) DFS(y, i-1, w);
    if(t[y][i] == t[y-1][i-1] && !odp[y-1][i-1]) DFS(y-1, i-1, w);
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, p, k, l, h, a, b, c, d;
    cin >> n >> m >> p;
    for(int i = 1; i <= n; i++)
        for(int y = 1; y <= n; y++)
            t[i][y] = 1;
    for(int i = 0; i < m; i++){
        cin >> k >> l >> h;
        for(int y = l; y <= h; y++)
            t[k][y] = 2;
    }
    for(int i = 1; i <= n; i++)
        for(int y = 1; y <= n; y++)
            if(!odp[i][y]){
                DFS(i, y, wart);
                wart++;
            }
    for(int i = 0; i < p; i++){
        cin >> a >> b >> c >> d;
        if(odp[a][b] == odp[c][d]) cout << "TAK" << '\n';
        else cout << "NIE" << '\n';
    }
}
