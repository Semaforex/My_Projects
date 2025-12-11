#include <bits//stdc++.h>
using namespace std;
bool sprawdz(int x, int najb[], int n){
    int w;
    if(najb[0]==-1) w=0;
    else{
        int i=najb[0];
        w=1;
        while(najb[i+x]!=-1 && i+x<n){
            w++;
            i=najb[i+x];
        }
    }
    if(w>=x) return true;
    else return false;
}
int main(){
    int n, ob=-1, wynik=0, sr;
    cin >> n;
    int t[n], szc[n], najb[n], k=sqrt(n)+1, p=0;
    for (int i=0; i<n; i++){
        cin >> t[i];
        szc[i]=0;
    }
    for (int i=1; i<n-1; i++)
        if(t[i - 1]<t[i] && t[i]>t[i + 1])
            szc[i]++;
    for (int i=n-1; i>=0; i--){
        if(szc[i] == 1) ob = i;
        najb[i] = ob;
    }
    while (k>=p){
        sr=(k+p)/2;
        if(sprawdz(sr, najb, n)){
            wynik=sr;
            p=sr+1;
        }
        else k=sr-1;
    }

    cout << wynik;

}
{
{1a, 2a, 3a, 4a}
{2a, }
}
A-2-3-5-B
 \     /
  1---
  |
  C

