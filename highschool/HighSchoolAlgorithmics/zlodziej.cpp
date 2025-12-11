#include <bits//stdc++.h>
using namespace std;
int c[1001], c2[1001], w[1001], w2[1001];
long long dp[1001][10001], dp2[1001][10001];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    for(int i = 1; i <= n; i++) cin >> c[i];
    for(int i = 1; i <= n; i++) cin >> w[i];
    for(int y = 1; y <= n; y++){
        for(int i = 1; i <= m; i++){
            dp[y][i] = dp[y-1][i];
            if(i-c[y] >= 0)
                dp[y][i] = max(dp[y][i], dp[y-1][i-c[y]]+w[y]);
        }
    }
    for(int i = 1; i <= n; i++){
        c2[i] = c[n-i+1];
        w2[i] = w[n-i+1];
    }
    for(int y = 1; y <= n; y++){
        for(int i = 1; i <= m; i++){
            dp2[y][i] = dp2[y-1][i];
            if(i-c2[y] >= 0)
                dp2[y][i] = max(dp2[y][i], dp2[y-1][i-c2[y]]+w2[y]);
        }
    }
    for(int i = 1; i <= n; i++){
        long long wyn = 0;
        for(int y = 0; y <= m; y++)
            wyn = max(wyn, dp[i-1][y]+dp2[n-i][m-y]);
        cout << wyn << ' ';
    }
}

