#include <iostream>
#include <vector>
using namespace std;

int n;
vector<int> graf[1001];
int odl[11][1001], poz[1001];

void wyznacz_ojcow(int x, int ojc) {
	odl[0][x] = ojc;
	for (int i = 0; i < graf[x].size(); i++) {
		int sas = graf[x][i];
		if (sas != ojc)
			wyznacz_ojcow(sas, x);
	}
}

void wypisz_ojcow(int poziom) {
	for (int i = 1; i <= n; i++)
		cout << odl[poziom][i] << " ";
	cout << endl;
}

void wyznacz_odleg() {
	int ile_poziomow = 10; // skok maksymalny to 2^10, czyli 1024, idealnie log z wysokoœci drzewa
	for (int poziom = 1; poziom <= ile_poziomow; poziom++) {
		for (int i = 1; i <= n; i++) {
			int tmp = odl[poziom - 1][i]; // pierwszy skok
			odl[poziom][i] = odl[poziom - 1][tmp];  // drugi skok
		}
	}
}

// dla kazdego wierzcholka zapisz na jakiej wysokosci sie znajduje
void wyznacz_wysokosci(int ob, int poziom) {
    poz[ob] = poziom;
    for(int i = 0; i < graf[ob].size(); i++){
        if(poz[graf[ob][i]] == 0)
            wyznacz_wysokosci(graf[ob][i], poziom+1);
    }
}

// wyznacza LCA wierzchiolka x i y
int LCA(int x, int y) {
    if(poz[x] < poz[y]) swap(x, y);
    int p = poz[y];
    for(int i = 0; i < )
}

int main() {
	cin >> n;
	for (int i = 0; i < n - 1; i++) {
		int a, b;
		cin >> a >> b;
		graf[a].push_back(b);
		graf[b].push_back(a);
	}
	wyznacz_wysokosci(1, 1);
	wyznacz_ojcow(1, 0);
	wyznacz_odleg();
	wypisz_ojcow(0);
	wypisz_ojcow(1);
	wypisz_ojcow(2);
}
