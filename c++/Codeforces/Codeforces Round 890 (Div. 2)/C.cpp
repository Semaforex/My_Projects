#include<bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    for(int q = 0; q < t; q++){
        int n, k;
        cin >> n >> k;
        int t[n], m;
        priority_queue<pair<pair<int, int>, int>> Q;
        cin >> t[0];
        cin >> t[1];
        m = max(t[0], t[1]);
        if(t[0] <= t[1]) Q.push({{t[0], -1}, 1});
        for(int i = 2; i < n; i++){
            cin >> t[i];
            if(t[i] >= t[i-1]) Q.push({{t[i-1], t[i-2]}, i});
            m = max(m, t[i]);
        }
        while(k>0 && !Q.empty()){
            int i = Q.top().second;
            int y = Q.top().first.first;
            Q.pop();
            if(y != t[i-1]) continue;
            if(t[i-1] <= t[i]){
                t[i-1]++;
                k--;
                m = max(m, t[i-1]);
            }
            if(i == 1){
                if(t[0] <= t[1]) Q.push({{t[0], -1}, 1});
            }
            else
                if(t[i-1] <= t[i]) Q.push({{t[i-1], t[i-2]}, i});
            if(i == 2)
                if(t[0] <= t[1]) Q.push({{t[0], -1}, 1});
            if(i > 2)
                if(t[i-2] <= t[i-1]) Q.push({{t[i-2], t[i-3]}, i-1});
        }
        cout << m << '\n';
        while(!Q.empty()) Q.pop();
    }
}
