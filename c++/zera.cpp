#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, m, x;
    cin >> n >> m;
    int t[n+1][m+1], z[n+1][m+1];
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++){
            cin >> x;
            z[i][j] = 0;
            if(x%10 == 0) z[i][j]++;
            if(x%100 == 0) z[i][j]++;
            if(x%1000 == 0) z[i][j]++;
            t[i][j] = x/pow(10, z[i][j]);
        }
    for(int i = 2; i <= n; i++) z[i][0] = 1000000;
    for(int i = 2; i <= n; i++) t[i][0] = 1;
    for(int i = 2; i <= m; i++) z[0][i] = 1000000;
    for(int i = 2; i <= m; i++) t[0][i] = 1;
    t[1][0] = 1;
    t[0][1] = 1;
    z[1][0] = 0;
    z[0][1] = 0;
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            int z1 = z[i][j-1]+z[i][j];
            int z2 = z[i-1][j]+z[i][j];
            int t1 = t[i][j-1]*t[i][j];
            int t2 = t[i-1][j]*t[i][j];
            for(int y = 0; y < 6; y++){
                if(t1%10 == 0){
                    z1++;
                    t1 /= 10;
                }
                if(t2%10 == 0){
                    z2++;
                    t2 /= 10;
                }
            }
            t[i][j] = t1%10000;
            z[i][j] = z1;
            if(z2 < z1){
                t[i][j] = t2%10000;
                z[i][j] = z2;
            }
        }
    }
    cout << z[n][m];
}
