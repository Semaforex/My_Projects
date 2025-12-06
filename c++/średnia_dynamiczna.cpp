#include <bits//stdc++.h>
using namespace std;
vector<int, int> t[1000000];
int s[1000000], odw[1000000];
void DFS(int ob, int oj, int i){
    odw[ob] = 1;
    if(ob != 1)
        t[oj].erase(i);
    for(int y = 0; y < t[ob].size(); y++){
        int syn  = t[ob][y];
        if(!odw[syn]) DFS(syn, ob, y);
    }
}
int main(){
    int n, x, y;
    cin >> n;
    for (int i = 0; i < n-1; i++){
        cin >> x >> y;
        t[x].push_back(y);
        t[y].push_back(x);
    }
    DFS(1, 1, 1);
    for(int i = 0; i < n-1; i++){
        for (int y = 0; y < t[i].size(); y++)
            cout << t[i][y] << ' ';
        cout << '\n';
    }
}
