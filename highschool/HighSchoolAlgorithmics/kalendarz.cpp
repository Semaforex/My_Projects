#include <bits//stdc++.h>
using namespace std;
pair<int, int> odpo(int dni,int t2[], int b){
    int sr = b/2, p = 1, k = b;
    while(!(dni-t2[sr-1] >= 0 && dni-t2[sr] <= 0)){
        if(t2[sr]-dni > 0){
            k = sr-1;
            sr = p+(k-p)/2;
        }
        else{
            p = sr+1;
            sr = p+(k-p)/2;
        }
    }
    return make_pair(sr, dni-t2[sr-1]);
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    int a[n+1], b[m+1], pa[n+1], pb[m+1];
    pa[0] = 0;
    pb[0] = 0;
    for(int i = 1; i <= n; i++){
        cin >> a[i];
        pa[i] = pa[i-1] + a[i];
    }
    for(int i = 1; i <= m; i++){
        cin >> b[i];
        pb[i] = pb[i-1] + b[i];
    }
    pair<int, int> odp;
    int mi, d, y, z;
    char k;
    cin >> z;
    for(int i = 0; i < z; i++){
        cin >> d >> mi >> k;
        if(k == 'A') odp = odpo(pa[mi-1]+d, pb, m);
        else odp = odpo(pb[mi-1]+d, pa, n);
        cout << odp.second << ' ' << odp.first << endl;
    }
}
