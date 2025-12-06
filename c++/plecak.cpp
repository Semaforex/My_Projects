#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    vector<int> v;
    int n, m;
    cin >> n >> m;
    int c[n+1], w[n+1];
    c[0] = 0;
    w[0] = 0;
    for(int i = 1; i <= n; i++) cin >> c[i];
    for(int i = 1; i <= n; i++) cin >> w[i];
    int dp[n+1][m+1];
    for(int y = 0; y <= m; y++) dp[0][y] = 0;
    for(int y = 0; y <= n; y++) dp[y][0] = 0;
    for(int y = 1; y <= n; y++){
        for(int i = 1; i <= m; i++){
            dp[y][i] = dp[y-1][i];
            if(i-c[y] >= 0)
                dp[y][i] = max(dp[y][i], dp[y-1][i-c[y]]+w[y]);
        }
    }
    int i = n, y = m;
    while(i > 0 && y > 0){
        if(dp[i][y] != dp[i-1][y]){
            v.push_back(i);
            y -= c[i];
        }
        i--;
    }
    cout << dp[n][m] << '\n' << v.size() << '\n';
    for(int i = 0; i < v.size(); i++) cout << v[i] << ' ';

}
