#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, a, b;
    cin >> n;
    vector<int> t[n+1];
    for (int i=1; i<n; i++){
        cin >> a >> b;
        t[a].push_back(b);
        t[b].push_back(a);
    }

}
