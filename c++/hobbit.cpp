#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, r, wynik = 0, q=0;
    cin >> n >> r;
    int m[n][n];
    for (int i=0; i<n; i++){
        for (int y=0; y<n; y++){
            m[y][i]=-1;
        }
    }
    string t[n];
    for (int i = 0; i < n; i++) cin >> t[i];
    for (int i = 0; i < n; i++) t[i][0] = t[i][0] + 'a'-'A';
    for (int i = 0; i < n; i++){
        for (int y = i-r; y <= i+r; y++){
            if(y >= 0 && y < n){
                if(m[i][y] == 1 && m[y][i] == 1){
                    wynik++;
                    break;
                }
                m[i][y] = 0;
                for (int z = 0; z < t[y].size(); z++){
                    for (int w = 0; w < t[i].size(); w++){
                        if(t[y][z] == t[i][w]){
                            wynik++;
                            m[i][y] = 1;
                            q = 1;
                            break;
                        }
                    }
                    if(q == 1){
                        q = 0;
                        break;
                    }
                }
            }
        }
        cout << wynik-1 << endl;
        wynik = 0;
    }
}
