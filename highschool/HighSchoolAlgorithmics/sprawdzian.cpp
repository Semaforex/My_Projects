#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    long long n, suma=0, minim;
    bool TAK = true;
    cin >> n;
    int t[n];
    for (int i=0; i<n; i++){
        cin >> t[i];
        suma += t[i];
    }
    minim = suma/n;
    while(TAK){
        for (int i=0; i<n; i++){
            if(t[i]<minim){
                suma += minim-t[i];
                t[i]=minim;
            }
        }
        if(minim == suma/n) TAK = false;
        minim = suma/n;
    }
    cout << minim;
}
