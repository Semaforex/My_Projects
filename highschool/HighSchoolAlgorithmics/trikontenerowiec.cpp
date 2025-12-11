#include <bits//stdc++.h>
using namespace std;
int t[1000001];
int s = 1;
void go_top(int x){
    t[s] = x;
    int oj = s/2, ob = s;
    while (t[ob] > t[oj]){
       swap(t[ob], t[oj]);
       ob /= 2;
       oj /= 2;
    }
    s++;
}
void Delete(){
    int ob = 1, syn = 2;
    t[ob] = 0;
    while(syn < s){
        syn = 2*ob + (t[2*ob] < t[2*ob+1]);
        swap(t[ob],t[syn]);
        ob = syn;
        syn = 2*ob;
    }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    t[0] = 1001;
    int m, n, wyn = 0;
    cin >> m >> n;
    pair<int, int> p[n];
    for (int i = 0; i < n; i++)
        cin >> p[i].second >> p[i].first;
    sort(p, p+n);
    int rozm = 1, ob = 0;
    while (rozm <= m){
        for(int i = ob; (p[i].first == rozm && i<n); i++){
            go_top(p[i].second);
            ob++;
        }
        wyn += t[1];
        Delete();
        rozm++;
    }
    cout << wyn;
}
