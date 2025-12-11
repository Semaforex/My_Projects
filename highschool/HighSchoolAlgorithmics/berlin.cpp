#include <bits//stdc++.h>
using namespace std;
int g[1000001], x;
int find_leader(int w){
	if(g[w] != w)
        g[w] = find_leader(g[w]);
	return g[w];
}

void union_leader(int a, int b){
    int lida = find_leader(a), lidb = find_leader(b);
    if(lida != lidb)
        x--;
    g[lidb] = lida;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, k;
    cin >> n >> m >> k;
    x = n;
    int a[m], b[m];
    for(int i = 1; i <= n; i++)
        g[i] = i;
    for(int i = 0; i < m; i++){
        cin >> a[i] >> b[i];
    }
    for(int i = m-1; i >=0; i--){
        union_leader(a[i], b[i]);
        if(x < k){
            cout << i+2;
            return 0;
        }
    }
}
