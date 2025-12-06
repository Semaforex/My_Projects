#include <bits//stdc++.h>
using namespace std;
vector<int> graf[500001];
int odl[500001];
bool odw[500001];

void DFS(int x, int dlu) {
    odw[x] = true;
    odl[x] = dlu;
    for (int i = 0; i<graf[x].size(); i++) {
        int sasiad = graf[x][i];
        if (!odw[sasiad]) {
            DFS(sasiad, dlu + 1);
        }
    }
}

int main(){
    int n, m, x, y;
    cin >> n >> m;
    for (int i = 1; i <= n-1; i++){
        cin >> x >> y;
        graf[x].push_back(y);
        graf[y].push_back(x);
    }
    for (int i = 1; i <= n; i++)
        odw[i] = false;
    DFS(1, 0);
    int maxim = odl[1], kon1 = 1;
    for (int i = 1; i <= n; i++){
       if(odl[i] > maxim){
            kon1 = i;
            maxim = odl[i];
       }
    }
    for (int i = 1; i <= n; i++)
        odw[i] = false;
    DFS(kon1, 0);
    maxim = odl[kon1];
    int kon2 = kon1;
    for (int i = 1; i <= n; i++){
       if(odl[i] > maxim){
            kon2 = i;
            maxim = odl[i];
       }
    }
    int wyn;
    if(m < odl[kon2]){
        wyn = m+1;
    }
    else{
        wyn = min(1+odl[kon2]+(m-odl[kon2])/2, n);
    }
   cout << wyn;
}
