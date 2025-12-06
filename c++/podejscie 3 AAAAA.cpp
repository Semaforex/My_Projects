#include <bits//stdc++.h>
using namespace std;
vector<int> v[200001];
int k, w;
void DFS(int ob, int oj, int odl){
    if(odl == k)
        w++;
    else
        for(int i = 0; i < v[ob].size(); i++)
            if(v[ob][i] != oj)
                DFS(v[ob][i], ob, odl+1);
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, a, b;
    cin >> n >> k;
    for(int i = 1; i < n; i++){
        cin >> a >> b;
        v[a].push_back(b);
        v[b].push_back(a);
    }
    for(int i = 1; i <= n; i++)
        DFS(i, i, 0);
    cout << w/2;
}
