#include <bits//stdc++.h>
using namespace std;
void przywroc(int i, int w, pair<int, bool>[]){
    int lewy=2*1, prawy=2*i+1, maks=i;
    if(lewy <= w && kop[lewy].first < kop[maks].first) maks = lewy;
    if(lewy <= w && kop[prawy].first < kop[maks].first) maks = prawy;
    if(maks!=i) przywroc(maks, w);
}
int main(){
    int n, k;
    cin >> n >> k;
    int kop[k+1], t[n];
    for (int i=0; i<n; i++) cin >> t[i];



}
