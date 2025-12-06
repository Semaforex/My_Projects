#include <bits//stdc++.h>
using namespace std;
int st_wej[1001];
int main(){
    int n, m; //ilość pkt, ilość krawędzi
    cin >> n >> m;
    vector<int> t[n+1], wyn;
    int a, b;
    for (int i = 0; i < m; i++){
        cin >> a >> b;
        t[a].push_back(b);
        st_wej[b]++;
    }
    queue<int> Q;
    for (int i = 1; i <= n; i++)
        if(st_wej[i] == 0) Q.push(i);
    while(!Q.empty()){
        int ob = Q.front();
        Q.pop();
        wyn.push_back(ob);
        for (int i = 0; i < t[ob].size(); i++){
            st_wej[t[ob][i]]--;
            if(st_wej[t[ob][i]] == 0) Q.push(t[ob][i]);
        }
    }
    for (int i = 0; i < n; i++) cout << wyn[i] << ' ';
}
