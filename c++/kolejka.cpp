#include <bits//stdc++.h>
using namespace std;
void DFS(int a, pair<vector<int>, int> v[], int k){
    v[a].second=k;
    for (int i=0; i<v[a].first.size(); i++){
        int s=v[a].first[i];
        if(v[s].second==-1) DFS(s, v, k);
    }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m, a, b;
    cin >> n >> m;
    pair<vector<int>, int> v[n+1];
    for (int i=0; i<m; i++){
        cin >> a >> b;
        v[a].first.push_back (b);
        v[b].first.push_back (a);
    }
    for (int i=0; i<=n; i++) v[i].second=-1;
    int z;
    cin >> z;
    for (int i=0; i<z; i++){
        cin >> a >> b;
        if(v[b].second==v[a].second && v[b].second!=-1) cout << "TAK";
        else{
            if((v[b].second!=-1 && -1!=v[a].second) || (v[b].second==-1 && v[a].second!=-1) || (v[b].second!=-1 && v[a].second==-1)) cout << "NIE";
            else{
                DFS(a, v, i);
                if(v[b].second==v[a].second) cout << "TAK";
                else cout << "NIE";
            }
        }
        cout << endl;
    }
}
