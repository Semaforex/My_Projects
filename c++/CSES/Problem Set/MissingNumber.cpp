#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    long long n, x;
    cin >> n;
    long long s = 0;
    for(int i = 1; i < n; i++){
        cin >> x;
        s += x;
    }
    cout << n*(n+1)/2-s;
}
