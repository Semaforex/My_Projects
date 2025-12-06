#include <iostream>
#include <vector>
using namespace std;
vector<int> g[1000001];
int odw[1000001], najd, wierz;
void DFS(int ob, int w){
    if(w > najd){
        najd = w;
        wierz = ob;
    }
    odw[ob]++;
    for(int i = 0; i < g[ob].size(); i++){
        if(!odw[g[ob][i]])
            DFS(g[ob][i], w+1);
    }
}
int main(){
    int n, a, b;
    cin >> n;
    for(int i = 1; i < n; i++){
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    DFS(1, 0);
    DFS(wierz, 0);
    cout << najd;
}
