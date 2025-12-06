#include <bits//stdc++.h>
using namespace std;
vector<int> v[200001];
int k, n;
long long add(deque<int> &a, deque<int> &b){
    if(a.size() < b.size())
        swap(a, b);
    int wa = a.size(), wb = b.size();
    long long w = 0;
    for(int i = max(0, k-wb+1); i < min(k+1, wa); i++)
        w += a[i]*b[k-i];
    for(int i = 0; i < wb; i++)
        a[i] += b[i];
    return w;
}
pair<long long, deque<int>> wynik(int ob, int oj){
    pair<long long, deque<int>> w = {0, {1}};
    for(int i = 0; i < v[ob].size(); i++){
        int sas = v[ob][i];
        if(sas != oj){
            pair<long long, deque<int>> pop = wynik(sas, ob);
            pop.second.push_front(0);
            w.first += add(w.second, pop.second)+pop.first;
        }
    }
    return w;
}
int main(){
    int a, b;
    cin >> n >> k;
    for(int i = 1; i < n; i++){
        cin >> a >> b;
        v[a].push_back(b);
        v[b].push_back(a);
    }
    pair<long long, deque<int>> wyn = wynik(1, 0);
    cout << wyn.first;
}
