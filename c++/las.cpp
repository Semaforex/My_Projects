#include<iostream>
#include<vector>
using namespace std;

int n, d;
int t[1002][1002];
bool odw[1002][1002];

void czysc() {
    for (int w = 0; w <= n + 1; w++)
        for (int k = 0; k <= n + 1; k++) {
            if (w == 0 || k == 0 || w == n + 1 || k == n + 1)
                odw[w][k] = true;
            else
                odw[w][k] = false;
        }
}

int BFS(int w, int k, int sr) {
    odw[w][k] = true;
    vector< pair<int, int> > v;
    v.push_back(make_pair(w, k));
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    for (int i = 0; i < v.size(); i++) {
        int akt_w = v[i].first;
        int akt_k = v[i].second;
        for (int j = 0; j < 4; j++) {
            int nowe_w = akt_w + dy[j];
            int nowe_k = akt_k + dx[j];
            if (t[nowe_w][nowe_k] <= sr && odw[nowe_w][nowe_k] == false) {
                v.push_back(make_pair(nowe_w, nowe_k));
                odw[nowe_w][nowe_k] = true;
            }
        }
    }
    return v.size();
}

bool sprawdz(int sr) {
    for (int w = 1; w <= n; w++) {
        for (int k = 1; k <= n; k++) {
            if (t[w][k] <= sr && odw[w][k] == false) {
                int ile = BFS(w, k, sr);
                if (ile >= d)
                    return true;
            }
        }
    }
    return false;
}

int main() {
    cin >> n >> d;
    for (int w = 1; w <= n; w++)
        for (int k = 1; k <= n; k++)
            cin >> t[w][k];

    int poc = 1;
    int kon = 1000 * 1000 * 1000;
    int wynik = kon;
    while (poc <= kon) {
        czysc();
        int sr = (poc + kon) / 2;
        if (sprawdz(sr)) {
            kon = sr - 1;
            wynik = sr;
        } else {
            poc = sr + 1;
        }
    }
    cout << wynik;
    return 0;
}
