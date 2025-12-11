#include <bits//stdc++.h>
using namespace std;
bool com(string a, string b){
    if(a.size() == b.size()) return a < b;
    return a.size() < b.size();
}
int main(){
    int n;
    cin >> n;
    string t[n];
    for (int i = 0; i < n; i++) cin >> t[i];
    sort(t, t+n, com);
    for (int i = 0; i < n; i++) cout << t[i] << '\n';
}
