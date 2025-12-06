#include <bits//stdc++.h>
using namespace std;
int main(){
    bool czy = false;
    int n, m, x;
    cin >> n >> m;
    int w2[n+1][m+1], w5[n+1][m+1];
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++){
            cin >> x;
            int c = 512, d = 9;
            while(x%c != 0){
                c/=2;
                d--;
            }
            w2[i][j] = d;
            c = 625;
            d = 4;
            while(x%c != 0){
                c/=5;
                d--;
            }c
            w5[i][j] = d;
            if(x == 0)
                czy = true;
        }
    w2[0][1] = 0;
    w2[1][0] = 0;
    w5[0][1] = 0;
    w5[1][0] = 0;
    for(int i = 2; i <= n; i++){
        w2[i][0] = 1000000;
        w5[i][0] = 1000000;
    }
    for(int i = 2; i <= m; i++){
        w2[0][i] = 1000000;
        w5[0][i] = 1000000;
    }
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++){
            w2[i][j] = min(w2[i][j]+w2[i-1][j], w2[i][j]+w2[i][j-1]);
            w5[i][j] = min(w5[i][j]+w5[i-1][j], w5[i][j]+w5[i][j-1]);
        }
    if(czy)
        cout << min(min(w2[n][m], w5[n][m]), 1);
    else
        cout << min(w2[n][m], w5[n][m]);
}
