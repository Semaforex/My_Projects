#include<iostream>
#include<vector>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t, n, k, x;
    cin >> t;
    for(int i = 0; i < t; i++){
        cin >> n >> k >> x;
        if(x != 1){
            cout << "YES" << '\n' << n << '\n';
            for(int y = 0; y < n; y++) cout << 1 << ' ';
            cout << '\n';
            continue;
        }
        pair<bool, int> tab[n+1];
        tab[1].first = false;
        for(int y = 2; y <= k; y++){
            tab[y].first = true;
            tab[y].second = -1;
        }
        for(int y = k+1; y <= n; y++){
            tab[y].first = false;
            for(int z = 2; z <= k; z++){
                if(tab[y-z].first){
                    tab[y].first = true;
                    tab[y].second = z;
                    break;
                }
            }
        }
        if(tab[n].first){
            vector<int> v;
            int cur = n;
            while(tab[cur].second != -1){
                v.push_back(tab[cur].second);
                cur -= tab[cur].second;
            }
            v.push_back(cur);
            cout << "YES" << '\n' << v.size() << '\n';
            for(int y = 0; y < v.size(); y++) cout << v[y] << ' ';
        }
        else cout << "NO";
        cout << '\n';
    }
}
