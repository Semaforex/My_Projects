#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, r, najmdot = 1000002;
    cin >> n >> r;
    float odl[n];
    r = r*r;
    float a, b;
    int wynik = 1000002;
    for (int i = 0; i < n; i++){
        cin >> a >> b;
        odl[i] =a*a + b*b;
    }
    for (int i = 0; i < n; i++){
        if(odl[i] <= r){
            cout << 1+i;
            return 0;
        }
    }
    cout << -1;
}
