#include<vector>
#include<iostream>
using namespace std;

vector<pair<int, int> > graf[1000001];
int suma;
bool odw[1000001];

void DFS(int x) {
    odw[x] = true;
    for(int i = 0; i < graf[x].size(); i++){
        int sasiad = graf[x][i].first;
        suma += graf[x][i].second;
        if(!odw[sasiad])
            DFS(sasiad);
    }
}

int main() {
    // wczytanie grafu
    int n, m, a, b, c;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        graf[a].push_back(make_pair(b, c));
        graf[b].push_back(make_pair(a, c));
    }
    int wynik = 0;
    for (int i = 1; i <= n; i++) {
        if (!odw[i]) {
            suma = 0;
            DFS(i);
            wynik = max(wynik, suma / 2);
        }
    }
    cout << wynik;
    return 0;
}
