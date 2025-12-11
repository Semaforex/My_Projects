#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    long long cur, x, s = 0;
    cin >> cur;
    for(int i = 1; i < n; i++){
        cin >> x;
        if(x < cur) s += cur-x;
        else cur = x;
    }
    cout << s;
}
