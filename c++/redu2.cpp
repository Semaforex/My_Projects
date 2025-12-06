#include <bits//stdc++.h>
using namespace std;
int main(){
    long long n, w = 0;
    cin >> n;
    int t[n];
    for(int i = 0; i < n; i++)
        cin >> t[i];
    w += t[0]-t[1];
    for(int i = 2; i < n; i++)
        w += abs(t[i]);
    cout << w;
}
