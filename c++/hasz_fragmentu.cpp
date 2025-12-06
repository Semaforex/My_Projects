#include <iostream>
#include <cmath>
using namespace std;
int main(){
    long long p = 31, mod = 1000000007;
    string s;
    cin >> s;
    long long t[s.size()+1], pot[s.size()];
    //tablica potêg
    pot[0] = 1;
    for(int i = 1; i < s.size(); i++)
        pot[i] = (pot[i-1]*p)%mod;
    //hasze sufiksów
    t[s.size()] = 0;
    for(int i = s.size()-1; i >= 0; i--)
        t[i] = (s[i]+t[i+1]*p)%mod;
    //hasze fragmentów
    int z, a, b;
    cin >> z;
    for(int i = 0; i < z; i++){
        cin >> a >> b;
        cout << t[a-1] - (t[b]*pot[b-a+1])%mod;
    }
}
