#include<iostream>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    for(int p = 0; p < t; p++){
        int n, m;
        bool can = true;
        cin >> n >> m;
        char t[n][m];
        for(int i = 0; i < n; i++){
            int c = 0;
            for(int y = 0; y < m; y++){
                cin >> t[i][y];
                if(t[i][y] == 'U' || t[i][y] == 'D') c++;
            }
            if(c%2) can = false;
        }
        for(int y = 0; y < m; y++){
            int c = 0;
            for(int i = 0; i < n; i++){
                if(t[i][y] == 'L' || t[i][y] == 'R') c++;
            }
            if(c%2) can = false;
        }
        if(!can){
            cout << -1 << '\n';
            continue;
        }
        char col[n][m];
        int cb = 1;
        for(int i = 0; i < n; i++){
            for(int y = 0; y < m; y++){
                if(t[i][y] == '.') col[i][y] = '.';
                if(t[i][y] == 'U'){
                    col[i][y] = 'W';
                    col[i+1][y] = 'B';
                    if(cb == 1) swap(col[i][y], col[i+1][y]);
                    cb *= -1;
                }
            }
        }
        for(int y = 0; y < m; y++){
            for(int i = 0; i < n; i++){
                if(t[i][y] == 'L'){
                    col[i][y] = 'W';
                    col[i][y+1] = 'B';
                    if(cb == 1) swap(col[i][y], col[i][y+1]);
                    cb *= -1;
                }
            }
        }
        for(int i = 0; i < n; i++){
            for(int y = 0; y < m; y++)
                cout << col[i][y];
            cout << '\n';
        }
    }
}
