#include <bits//stdc++.h>
using namespace std;
vector<int> v[200001], wynik;
int odl[200001];
int DFS3(int ob, int oj){
    int sum = 1;
    for(int i = 0; i < v[ob].size(); i++)
        if(v[ob][i] != oj)
            sum += DFS3(v[ob][i], ob);
    return sum;
}
long long DFS2(int ob, int k, int oj){
    if(k == 0) return 1;
    long long sum = 0;
    for(int i = 0; i < v[ob].size(); i++)
        if(v[ob][i] != oj)
            sum += DFS2(v[ob][i], k-1, ob);
    return sum;
}
long long DFS(int ob, int k, int oj){
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
    return sum;
}

int main(){
    //ios_base::sync_with_stdio(0);
    //cin.tie(0);
    int n, k, a, b, ob = 1, oj = 1;
    bool good = true;
    cin >> n >> k;
    for(int i = 1; i < n; i++){
        cin >> a >> b;
        v[a].push_back(b);
        v[b].push_back(a);
    }
    while(good){
        good = false;
        for(int i = 0; i < v[ob].size(); i++){
            if(v[ob][i] != oj)
                if(DFS3(v[ob][i], ob) > n/2){
                    ob = v[ob][i];
                    oj = ob;
                    i = v[ob].size();
                    good = true;
                }
        }
    }
    cout << ob << ' ' << DFS(ob, k, ob);
}
