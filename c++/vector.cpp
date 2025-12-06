#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    char a;
    int n, x;
    cin >> n;
    vector<int> v;
    for(int i = 0; i < n; i++){
        cin >> a;
        if(a == '-'){
            cout << v[v.size()-1] << '\n';
            v.pop_back();
        }
        if(a == '+'){
            cin >> x;
            v.push_back(x);
        }
    }
}
