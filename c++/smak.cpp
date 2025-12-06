#include <iostream>
using namespace std;
int main(){
    long long n, m, p=0, w=0;
    bool powtorka=false;
    cin >> n >> m;
    int t[n];
    int tab[m+1];
    for (int i=0; i<n; i++) cin >> t[i];
    for (int i=0; i<=m; i++) tab[i]=0;
    for (int k=0; k<n; k++){
        while(p<n && (!powtorka)){
            if(tab[t[p]]==0){
                tab[t[p]]=1;
                p++;
            }
            else{
                powtorka=true;
            }
        }
        w+=p-k;
        tab[t[k]]--;
        powtorka = false;
    }
    cout << w;
}
