#include <bits//stdc++.h>
using namespace std;
vector<int> v[200001];
long long DFS2(int ob, int k, int oj){
    if(k == 0) return 1;
    long long sum = 0;
    for(int i = 0; i < v[ob].size(); i++)
        if(v[ob][i] != oj)
            sum += DFS2(v[ob][i], k-1, ob);
    return sum;
}
long long DFS(int ob, int k, int oj){
   // if( == 1) return v[ob].size();
    long long sum = 0;
    for(int i = 0; i < v[ob].size(); i++){
        if(v[ob][i] != oj){
            sum += DFS(v[ob][i], k, ob);
            sum += DFS2(v[ob][i], k-1, ob);
            for(int j = i+1; j < v[ob].size(); j++)
                if(v[ob][j] != oj)
                    for(int y = 1; y <= k-1; y++)
                        sum += DFS2(v[ob][i], y-1, ob)*DFS2(v[ob][j], k-y-1, ob);
        }
    }
    //if(ob == 2) cout << ob << ' '<< sum << ' ';
    return sum;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, k, a, b;
    cin >> n >> k;
    for(int i = 1; i < n; i++){
        cin >> a >> b;
        v[a].push_back(b);
        v[b].push_back(a);
    }
    cout << DFS(1, k, 0);
}
