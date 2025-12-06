#include<iostream>
#include<algorithm>
#include<climits>
using namespace std;

pair<int, int> dom[10000001];

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> dom[i].first >> dom[i].second;
    sort(dom, dom + n);
    long long wynik = LLONG_MAX;
    long long odleglosc = dom[0].second;
    for (int i = 1; i < n; i++) {
        odleglosc += (dom[i].first - dom[i - 1].first);
        wynik = min(wynik, odleglosc + dom[i].second);
        odleglosc = min(odleglosc, (long long)dom[i].second);
    }
    cout << wynik;
}
