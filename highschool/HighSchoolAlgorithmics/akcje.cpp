#include <bits//stdc++.h>
using namespace std;
vector<int> zliczNieprzedluzalne(int t[],int n){
    vector<int> Nieprzedluzalne;
    int dlugosc=1;
    for (int i=0; i<=n; i++) Nieprzedluzalne.push_back(0);
    for (int i=0; i<n; i++){
        if(t[i]<t[i+1]) dlugosc+=1;
        else{
            Nieprzedluzalne[dlugosc]++;
            dlugosc=1;
        }
    }
    return Nieprzedluzalne;
}
vector<int> sumuj(int n, vector<int> niep){
    vector<int> wyniki;
    for (int i=0; i<=n+1; i++) wyniki.push_back(0);
    int suma=0;
    for (int i=n; i>0; i--){
        suma+=niep[i];
        wyniki[i]=wyniki[i+1]+suma;
    }
    return wyniki;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n;
    cin >> n;
    int t[n+1];
    t[n]=0;
    for (int i=0; i<n; i++) cin >> t[i];
    vector<int> niep = zliczNieprzedluzalne(t, n);
    vector<int> wyniki = sumuj(n, niep);
    int q;
    cin >> q;
    for (int i=0; i<q; i++){
        cin >> n;
        cout << wyniki[n] << endl;
    }
}
