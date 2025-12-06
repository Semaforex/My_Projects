#include <bits//stdc++.h>
using namespace std;
int n, odw[100*100*100], parzyst = 0, nieparzyst = 0;
int DFS(vector<int> t[], int ob, int ktory){
    odw[ob] = 1;
    if(ktory == 1) parzyst++;
    else nieparzyst++;
    ktory *= -1;
    for(int i = 0; i < t[ob].size(); i++){
        if(odw[t[ob][i]] == 0) DFS(t, t[ob][i], ktory);
    }
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int a, b;
    cin >> n;
    vector<int> t[n+1];
    for (int i = 1; i < n; i++){
        cin >> a >> b;
        t[a].push_back(b);
        t[b].push_back(a);
    }
    DFS(t, 1, 1);
    int p, np;
    cout << n << " ";
    for (int i = 2; i <= n; i++){
        if(parzyst % i == 0) p = parzyst/i;
        else p = parzyst/i + 1;
        if(nieparzyst % i == 0) np = nieparzyst/i;
        else np = nieparzyst/i + 1;
        cout << np + p << " ";
    }
}
