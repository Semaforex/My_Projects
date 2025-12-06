#include<iostream>
using namespace std;
long long f[87];
int wynik(long long n, int wyn, int ac){
        int w = 1000000;
        for(int i = ac; i < 87; i++){
            if(n == f[i]) return wyn+i+1;
            if(n%f[i] == 0){
                //cout << f[i] << ' ' << n/f[i] << ' ';
                w = min(w, wynik(n/f[i], wyn+i+1, i));
            }
        }
        return w;
}
int main(){
    f[0] = 1;
    f[1] = 1;
    for(int i = 2; i < 87; i++)
        f[i] = f[i-1]+f[i-2];
    long long n;
    cin >> n;
    int suma = 0;
        if(n == 1){
            cout << 1;
            return 0;
        }
        int w = wynik(n, -1, 2);
        if(w == 1000000) cout << "NIE";
        else cout << w;
}
