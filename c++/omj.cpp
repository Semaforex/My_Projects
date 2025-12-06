#include <iostream>
using namespace std;
int main(){
    int n, m, x, y;
    cin >> n >> m;
    int pref[n+1];
    pref[0]=0;
    for (int i=1; i<=n; i++){
        cin >> x;
        pref[i]=pref[i-1]+x;
    }
    for (int i=0; i<m; i++){
        cin >> x >> y;
        cout << pref[y]-pref[x-1] << endl;
    }
}
