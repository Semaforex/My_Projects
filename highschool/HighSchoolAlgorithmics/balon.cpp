#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    pair<int, int> t[n];
    for (int i = 0; i<n; i++) cin >> t[i].first >> t[i].second;
    int g = t[0].first+t[0].second, d = t[0].first-t[0].second;
    for (int i=1; i<n; i++){
        g = min(t[i].second+t[i].first, g+1);
        d = max(t[i].first-t[i].second, d-1);
    }
    cout << d << " " << g << endl;

}
