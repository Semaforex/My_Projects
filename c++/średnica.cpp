#include <bits/stdc++.h>
using namespace std;

int odl[1000001];
bool odw[1000001];
vector<int> graf[1000001];

void DFS(int x, int dlu) {
    odw[x] = true;
    odl[x] = dlu;
    // idziemy po wszystkich sasiadach
    for (int i = 0; i < graf[x].size(); i++) {
        int sasiad = graf[x][i];
        if (!odw[sasiad]) {
            DFS(sasiad, dlu + 1);
        }
    }
}

int main() {
    int n, a, b;
    // wczytanie grafu
    cin >> n;
    for (int i = 0; i < n - 1; i++) {
        cin >> a >> b;
        graf[a].push_back(b);
        graf[b].push_back(a);
    }
    // szukamy pierwszego konca srednicy
    DFS(1, 0);
    int s1 = 1;
    for (int i = 1; i <= n; i++) {
        if (odl[i] > odl[s1])
            s1 = i;
    }
    // zerowanie tablic
    for (int i = 1; i <= n; i++) {
        odw[i] = false;
        odl[i] = 0;
    }
    // szukamy drugiego konca srednicy
    DFS(s1, 0);
    int wynik = 0;
    for (int i = 1; i <= n; i++)
        wynik = max(wynik, odl[i]);
    cout << wynik;
    return 0;
}
