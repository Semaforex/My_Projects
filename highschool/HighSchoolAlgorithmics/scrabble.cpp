#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int a, b;
    cin >> a >> b;
    char mer[a], pip[b];
    for (int i = 0; i < a; i++){
        cin >> mer[i];
    }
    for (int i = 0; i < b; i++){
        cin >> pip[i];
    }
    int i = 0, ob = 97, wynik = 0;
    while (i < a && ob < 123){
        if(mer[i] == char(ob)){
            ob++;
            wynik++;
        }
        i++;
    }
    cout << "Merry " << wynik << endl;
    wynik = 0;
    ob = 97;
    i = 0;
    while (i < b && ob < 123){
        if(pip[i] == char(ob)){
            ob++;
            wynik++;
        }
        i++;
    }
    cout << "Pippin " << wynik << endl;
}
