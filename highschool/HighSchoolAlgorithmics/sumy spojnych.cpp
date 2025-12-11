#include<vector>
#include<iostream>
using namespace std;

vector<int> graf[1000001];
int suma;
bool odw[1000001];

void DFS(int x) {
    odw[x] = true;
    suma += x;
    for(int i = 0; i < graf[x].size(); i++){
        int sasiad = graf[x][i];
        if(!odw[sasiad])
            DFS(sasiad);
    }
}

int main() {
    // wczytanie grafu
    int n, a, b;
    cin >> n;
    for (int i = 0; i < n - 1; i++) {
        cin >> a >> b;
        graf[a].push_back(b);
        graf[b].push_back(a);
    }
    int wynik = 0;
    for (int i = 1; i <= n; i++) {
        if (!odw[i]) {
            suma = 0;
            DFS(i);
            wynik = max(wynik, suma);
        }
    }
    cout << wynik;
    return 0;
}
