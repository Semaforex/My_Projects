#include <bits//stdc++.h>
using namespace std;
vector<int> d[1000000];
int odw[1000000];
int w = 0, wyn = 1;
bool parz = false;
void DFS(int ojc, int ob){
    w++;
    wyn = max(wyn, w);
    odw[ob]++;
    for(int i = 0; i < d[ob].size(); i++){
        if(odw[d[ob][i]] == 0){
            if(d[ob][i]%2 == 0){
                if(parz) continue;
                parz++;
            }
            DFS(ob, d[ob][i]);
        }
    }
    if(ob%2 == 0)
        parz = true;
    w--;
}
int main(){
    int n, m, a, b;
    cin >> n >> m;
    for (int i = 0; i < m; i++){
        cin >> a >> b;
        d[a].push_back(b);
        d[b].push_back(a);
    }
    DFS(0, 1);
    cout << wyn << '\n';
}
