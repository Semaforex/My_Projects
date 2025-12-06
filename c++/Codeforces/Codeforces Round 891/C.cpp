#include<bits//stdc++.h>
using namespace std;
int t[2*100000+1];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int q;
    cin >> q;
    for(int z = 0; z < q; z++){
        int n;
        cin >> n;
        int m = n*(n-1)/2;
        int t[m];
        for(int i = 0; i < m; i++)
            cin >> t[i];
        sort(t, t+m);
        int pop = t[0], c = 0;
        vector<pair<int, int>> v = {{pop, 1}};
        for(int i = 1; i < m; i++){
            if(t[i] != pop){
                pop = t[i];
                v.push_back({pop, 1});
                c++;
            }
            else v[c].second++;
        }
        int suf[c+1];
        suf[0] = m;
        for(int i = 1; i <= c+1; i++) suf[i] = suf[i-1]-v[i-1].second;
        for(int i = 0; i <= c; i++){
            int k;
            if(suf[i+1] != 0)k = (v[c].second-i*(i-1)/2)/(suf[i+1]);
            else k = 2;
            for(int y = 0; y < k; y++)
                cout << v[c].first << ' ';
        }
        cout << '\n';
    }
}
