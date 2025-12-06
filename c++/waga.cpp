#include <bits//stdc++.h>
using namespace std;
bool com(pair<int, int> a, pair<int, int> b){
    double w1 = a.first/pow(2, a.second), w2 = b.first/pow(2, b.second);
    if(w1 == w2) return a < b;
    return w1 < w2;
}
int main(){
    int n;
    cin >> n;
    pair <int, int> t[n];
    for(int i = 0; i < n; i++) cin >> t[i].first >> t[i].second;
    sort(t, t+n, com);
    for(int i = 0; i < n; i++) cout << t[i].first << ' ' << t[i].second << '\n';

}
