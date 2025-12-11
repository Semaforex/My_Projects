#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, m, w = 0;
    cin >> n >> m;
    int t[n][m];
    for(int i = 0; i < n; i++){
        for(int y = 0; y < m; y++){
            cin >> t[i][y];
        }
    }
    for(int i = 1; i < n; i++){
        for(int y = 1; y < m; y++){
            if(t[i][y] == 1)
                t[i][y] = min(t[i-1][y]+1, min(t[i-1][y-1]+1, t[i][y-1]+1));
            w = max(w, t[i][y]);
        }
    }
    cout << w;
}
