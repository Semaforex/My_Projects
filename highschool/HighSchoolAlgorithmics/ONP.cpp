#include <iostream>
using namespace std;
int zamien(string s){
    long long wynik = 0;
    for(int i = 0; i < s.size(); i++)
        wynik = 10*wynik+ int(s[i]-'0');
    return wynik;
}
int main(){
    long long n, a, b;
    cin >> n;
    string t[n];
    long long stos[n], r = 0;
    for(int i = 0; i < n; i++){
        cin >> t[i];
        if(t[i] == "+"){
            r--;
            a = stos[r-1];
            b = stos[r];
            stos[r-1] = a+b;
        }
        else if(t[i] == "-"){
            r--;
            a = stos[r-1];
            b = stos[r];
            stos[r-1] = a-b;
        }
        else if(t[i] == "*"){
            r--;
            a = stos[r-1];
            b = stos[r];
            stos[r-1] = a*b;
        }
        else if(t[i] == "/"){
            r--;
            a = stos[r-1];
            b = stos[r];
            stos[r-1] = a/b;
        }
        else{
            stos[r] = zamien(t[i]);
            r++;
        }

    }
    cout << stos[0];
}
