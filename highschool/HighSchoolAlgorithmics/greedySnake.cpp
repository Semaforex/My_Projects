#include<bits//stdc++.h>
using namespace std;
int t[51][51], dp[101][51][51][51];
int main(){
    int n, m;
    char x;
    cin >> n >> m;
    for(int i = 1; i <= n; i++)
        for(int y = 1; y <= m; y++){
            cin >> x;
            t[y][i] = int(x-'0');
        }
    for(int i = 1; i <= 50; i++){
        for(int a = 1; a <= i; a++){
            for(int b = a; b <= i; b++){
                for(int c = b; c <= i; c++){
                    int ay = i-a, by = i-b, cy = i-c;
                    dp[i][a][b][c] = max({
                    dp[i-1][a][b][c],
                    dp[i-1][a][b-1][c],
                    dp[i-1][a][b][c-1],
                    dp[i-1][a][b-1][c-1],
                    dp[i-1][a-1][b][c],
                    dp[i-1][a-1][b-1][c],
                    dp[i-1][a-1][b][c-1],
                    dp[i-1][a-1][b-1][c-1]
                    });
                    int p = t[a][ay], q = t[b][by];
                    dp[i][a][b][c] += t[a][ay];
                    t[a][ay] = 0;
                    dp[i][a][b][c] += t[b][by];
                    t[b][by] = 0;
                    dp[i][a][b][c] += t[c][cy];
                    t[a][ay] = p;
                    t[b][by] = q;
                }
            }
        }
    }
    for(int i = 51; i <= 100; i++){
        for(int a = i-50; a <= 50; a++){
            for(int b = a; b <= 50; b++){
                for(int c = b; c <= 50; c++){
                    int ay = i-a, by = i-b, cy = i-c;
                    dp[i][a][b][c] = max({
                    dp[i-1][a][b][c],
                    dp[i-1][a][b-1][c],
                    dp[i-1][a][b][c-1],
                    dp[i-1][a][b-1][c-1],
                    dp[i-1][a-1][b][c],
                    dp[i-1][a-1][b-1][c],
                    dp[i-1][a-1][b][c-1],
                    dp[i-1][a-1][b-1][c-1]
                    });
                    int p = t[a][ay], q = t[b][by];
                    dp[i][a][b][c] += t[a][ay];
                    t[a][ay] = 0;
                    dp[i][a][b][c] += t[b][by];
                    t[b][by] = 0;
                    dp[i][a][b][c] += t[c][cy];
                    t[a][ay] = p;
                    t[b][by] = q;
                }
            }
        }
    }
    cout << dp[n+m][m][m][m];
}
