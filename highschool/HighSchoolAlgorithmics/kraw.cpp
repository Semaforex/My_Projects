#include <iostream>
#include <vector>
using namespace std;
int odw[1000001], w=0;
vector<pair<int, bool>> t[1000001];
void DFS(int ob){
    odw[ob]++;
    for (int i = 0; i < t[ob].size(); i++){
        int sas = t[ob][i].first;
        if(odw[sas] == 0){
            if(t[ob][i].second) w++;
            DFS(sas);
        }
    }
}
int main(){
    int n, a, b;
    cin >> n;
    for (int i = 0; i < n-1; i++){
        cin >> a >> b;
        t[a].push_back(make_pair(b, true));
        t[b].push_back(make_pair(a, false));
    }
    DFS(1);
    cout << w;
}
