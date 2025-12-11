#include <iostream>
#include <vector>
using namespace std;
vector<int> t[1000001];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, a, m, d;
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> a;
        t[a].push_back(i);
    }
    cin >> m;
    for(int i = 0; i < m; i++){
        cin >> d;
        int ost = -1;
        bool czy = true;
        for(int y = 0; y < d; y++){
            cin >> a;
            int p = 0, k = t[a].size()-1, sr, w = -1;
            while(p <= k){
                sr = (p+k)/2;
                if(t[a][sr] > ost){
                    k = sr-1;
                    w = t[a][sr];
                }
                else p = sr+1;
            }
            if(w == -1) czy = false;
            ost = w;
        }
        if(czy) cout << "TAK\n";
        else cout << "NIE\n";
    }
}
