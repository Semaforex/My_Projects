#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int Q = 0; Q < T; Q++){
        long long n, k, x;
        cin >> n >> k >> x;
        if(n*(n+1)-(n-k)*(n-k+1) < 2*x || k*(k+1) > 2*x) cout << "no" << '\n';
        else cout << "yes" << '\n';
    }
}
