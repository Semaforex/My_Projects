#include <iostream>
#include <vector>
#include <queue>
using namespace std;

priority_queue<pair<int, int> > Q;
vector<pair<int, int> > v[1000 * 1000 + 1];
int wyn[2][1000 * 1000 + 1];
int mx = 1000 * 1000 * 1000 + 1;
int minim = 1000 * 1000 * 1000 + 1;
int wynik = 1;

void dijkstra(int pocz, int u){
    Q.push(make_pair(0, pocz)); // dodanie do kolejki prioryt. wierzcholka pocz¹tkowego z wartoscia 0

    // wlasciewe dzialanie Dijkstry
    while (!Q.empty()) {
        int w = Q.top().second; // numer wierzcholka
        int d = -Q.top().first;
        Q.pop();
        if (wyn[u][w] < d)
            continue;
    	for (int i = 0; i < v[w].size(); i++) {
            int sas = v[w][i].first;
            int odl = v[w][i].second;
            if (wyn[u][sas] > d + odl) {
                wyn[u][sas] = d + odl;
                Q.push(make_pair(-wyn[u][sas], sas));
            }
    	}
    }
}

int main() {
	int n, m, pocz, kon;
	cin >> n >> m >> pocz >> kon;
	for (int i = 0; i < m; i++) {
		int a, b, c;
		cin >> a >> b >> c;
		v[a].push_back(make_pair(b, c));
		v[b].push_back(make_pair(a, c));
	}

	for(int i = 0; i <= n; i ++){
        wyn[0][i] = mx;
        wyn[1][i] = mx;
	}
	wyn[0][pocz] = 0;
	wyn[1][kon] = 0;

    dijkstra(pocz, 0);
    dijkstra(kon, 1);

    for (int i=1; i<=n; i++){
         if(max(wyn[0][i], wyn[1][i]) < minim){
            minim = max(wyn[0][i], wyn[1][i]);
            wynik = i;
         }
    }
    cout << wynik << " " << minim;
	return 0;
}
