#include <iostream>
using namespace std;
int main(){
    long long p = 31, mod = 1000000007;
    string s, w;
    cin >> s >> w;
    long long pot[s.size()];
    //tablica potêg
    pot[0] = 1;
    for(int i = 1; i < s.size(); i++) pot[i] = (pot[i-1]*p)%mod;
    //hasz wzorca i hasz pocz¹tku s o d³ugoœci wzorca
    long long haszW = 0, haszS = 0;
    for(int i = 0; i < w.size(); i++){
        haszW = (haszW + (w[i]*pot[i])%mod)%mod;
        haszS = (haszS + (s[i]*pot[i])%mod)%mod;
    }
    //zliczanie wyst¹pieñ
    int wynik = 0;
    if(haszS == haszW) wynik++;
    for(int i = 0; i < s.size()-w.size(); i++){
        haszS = (haszS - (s[i]*pot[i])%mod + s[i+w.size()]*pot[i+w.size()])%mod;
        haszW = (haszW*p)%mod;
        if(haszS == haszW) wynik++;
    }
    cout << wynik;
}
