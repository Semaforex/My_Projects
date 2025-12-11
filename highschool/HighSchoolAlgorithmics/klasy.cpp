#include <iostream>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int ZX[4] = {1, 0, -1, 0};
    int ZY[4] = {0, 1, 0, -1};
    int n, x1, x2, y1, y2, x, y, zx, zy, nr = 1;
    string K, S;
    cin >> n >> K >> S >> x1 >> x2 >> y1 >> y2;
    int t[x2-x1+1][y2-y1+1];
    if(K == "POLNOC" && S == "PRAWO"){x = 1; y = 1; zx = 3; zy = 1;}
    if(K == "POLNOC" && S == "LEWO"){x = n; y = 1; zx = 1; zy = 1;}
    if(K == "WSCHOD" && S == "PRAWO"){x = 1; y = n; zx = 0; zy = 2;}
    if(K == "WSCHOD" && S == "LEWO"){x = 1; y = 1; zx = 0; zy = 0;}
    if(K == "POLUDNIE" && S == "PRAWO"){x = n; y = n; zx = 1; zy = 3;}
    if(K == "POLUDNIE" && S == "LEWO"){x = 1; y = n; zx = 3; zy = 3;}
    if(K == "ZACHOD" && S == "PRAWO"){x = n; y = 1; zx = 2; zy = 0;}
    if(K == "ZACHOD" && S == "LEWO"){x = n; y = n; zx = 2; zy = 2;}
    for(int i = 1; i < 2*n; i++){
        for(int j = 1; j <= n-i/2; j++){
            if(x >= x1 && x <= x2 && y >= y1 && y <= y2)
                t[x-x1][y-y1] = nr;
            nr++;
            if(j == n-i/2){
                zx = (zx+1)%4;
                zy = (zy+1)%4;
            }
            x += ZX[zx];
            y += ZY[zy];
        }
    }
    for(int i = y2-y1; i >= 0; i--){
        for(int j = 0; j <= x2-x1; j++)
            cout << t[j][i] << ' ';
        cout << '\n';
    }

}
