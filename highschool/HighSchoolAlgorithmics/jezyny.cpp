#include <bits//stdc++.h>
using namespace std;
int sprawdz(string ow, int n){
    int suma=0;
    for (int i = 1; i<n; i++){
        if(ow[i-1] == '1' && ow[i] == '1'){
            ow[i-1] = '0';
            ow[i] = '0';
            suma++;
        }
    }
    return suma;
}
int main(){
    int n, q, A, B, M, wynik=0;
    cin >> n >> q;
    string ow;
    cin >> ow;
    cin >> A >> B >> M;
    int p[q];
    p[0] = 0;
    for (int i=1; i<q; i++){
        p[i] = (A * p[i-1] + B)%M;
    }
    for(int i = 0; i < q; i++){
        if(ow[p[i]%n] == '1')
            ow[p[i]%n] = '0';
        else
            ow[p[i]%n] = '1';
        wynik += sprawdz(ow, n);
    }
    cout << wynik;
}
