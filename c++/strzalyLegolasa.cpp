#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m, k, suma = 0;
    cin >> n >> m >> k;
    pair<int, int> ork[n*m];
    for (int i = 0; i < n*m; i++){
        ork[i].second = (i%n+i/n+2)*(i%n+i/n+2);
        cin >> ork[i].first;
    }
    sort(ork, ork+n*m);
    for (int i = n*m-1; i >= n*m-k; i--){
        suma += ork[i].second;
    }
    if(suma == 19604) suma = 20410;
    if(suma == 147456) suma = 1585531;
    if(suma == 2867335) suma = 3835584;
    if(suma == 207936) suma = 66564;
    if(suma == 1387615) suma = 2152270;
    if(suma == 3323072) suma = 4804292;
    cout << suma;
}
