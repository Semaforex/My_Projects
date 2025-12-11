#include <bits//stdc++.h>
using namespace std;
struct bakt{
    int lid;
    int IQ_MIN;
    int IQ_MAX;
    int masa;
};
bakt g[1000001];
int find_leader(int w){
	if(g[w].lid != w)
        return find_leader(g[w].lid);
	return w;
}

void union_leader(int a, int b){
    int lida = find_leader(a), lidb = find_leader(b);
    g[lidb].lid = lida;
    g[lida].IQ_MIN = min(g[lida].IQ_MIN, g[lidb].IQ_MIN);
    g[lida].IQ_MAX = max(g[lida].IQ_MAX, g[lidb].IQ_MAX);
    if(lida != lidb)
        g[lida].masa += g[lidb].masa;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, a, b;
    cin >> n;
    for(int i = 1; i <= n; i++){
        cin >> a >> b;
        g[i].lid = i;
        g[i].masa = a;
        g[i].IQ_MIN = b;
        g[i].IQ_MAX = b;
    }
    string p;
    while(cin >> p){
        if(p == "JOIN"){
            cin >> a >> b;
            union_leader(a, b);
        }
        if(p == "IQ_MIN"){
            cin >> a;
            cout << g[find_leader(a)].IQ_MIN << '\n';
        }
        if(p == "IQ_MAX"){
            cin >> a;
            cout << g[find_leader(a)].IQ_MAX << '\n';
        }
        if(p == "MASA"){
            cin >> a;
            cout << g[find_leader(a)].masa << '\n';
        }
    }
    return 0;
}
