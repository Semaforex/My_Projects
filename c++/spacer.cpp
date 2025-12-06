#include <iostream>
using namespace std;
int odleglosc(long long a, long long b){
    long long wynik=0;
    while(a!=b){
        if(a>b)
            a/=2;
        else
            b/=2;
        wynik++;
    }
    return wynik;
}
int main(){
    long long n, a, b;
    cin >> n;
    for (int i=0; i<n; i++){
       cin >> a >> b;
       cout << odleglosc(a, b) << endl;
    }
}
