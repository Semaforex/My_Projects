#include <iostream>
using namespace std;
int main(){
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    int n, m, x, y, z, w;
    cin >> n >> m;
    int pref[n+1][m+1];
    for (int i=0; i<=n; i++) pref[i][0]=0;
    for (int i=0; i<=m; i++) pref[0][i]=0;
    for (int i=1; i<=n; i++){
        for (int j=1; j<=m; j++){
                cin >> x;
                pref[i][j]=pref[i][j-1]+x+pref[i-1][j]-pref[i-1][j-1];

        }
    }



    cin >> n;
    for (int i=0; i<n; i++){
        cin >> y >> x >> w >> z;
        cout << pref[w][z]-pref[w][x-1]-pref[y-1][z]+pref[y-1][x-1] << endl;
    }
}
