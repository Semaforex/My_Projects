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
    int i = 0, ob = 97, wynik = 0, ostateczny = 0;
    for (int ob = 97; ob < 123; ob++){
        while (i < a && ob < 123){
            if(mer[i] == char(ob)){
                ob++;
                wynik++;
            }
            i++;
        }
        ostateczny = max(ostateczny, wynik);
        wynik = 0;
    }
    cout << "Merry " << ostateczny << endl;
    ostateczny = 0;
    ob = 97;
    i = 0;
    for (int ob = 97; ob < 123; ob++){
        while (i < a && ob < 123){
            if(pip[i] == char(ob)){
                ob++;
                wynik++;
            }
            i++;
        }
        ostateczny = max(ostateczny, wynik);
        wynik = 0;
    }
    cout << "Pippin " << wynik << endl;
}
