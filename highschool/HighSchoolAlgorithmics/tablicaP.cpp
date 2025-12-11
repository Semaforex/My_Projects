#include <iostream>
using namespace std;
int P[1000000], w;
bool sprawdz(int d){
    int i = w;
    while(i > 0){
        for(int y = d; y >= 0; y--){
            if(P[i] == d)
                y = -1;
            if(y == 0) return false;
            i--;
        }
    }
    return true;
}
int main(){
    string s;
    cin >> s;
    w = s.size();
    s = ' ' + s;
    P[1] = 0;
    int t = P[1];
    for (int i = 2; i <= w; i++) {
        while (t > 0 && s[t + 1] != s[i])
            t = P[t]; // poszukiwanie odpowiedniego prefikso-sufiksu
        if (s[t + 1] == s[i]) t++;
        P[i] = t;
    }
    if(sprawdz(P[w])) cout << "tak";
}
