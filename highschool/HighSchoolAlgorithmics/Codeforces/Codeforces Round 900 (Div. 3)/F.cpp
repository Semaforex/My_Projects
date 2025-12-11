#include<iostream>
using namespace std;
int divisors(int n){
    if(n == 1) return 1;
    int w = 2, i = 2;
    while(i*i < n){
        if(!(n%i))
            w += 2;
        i++;
    }
    if(i*i == n) w += 1;
    return w;
}

int main(){
    //ios_base::sync_with_stdio(0);
    //cin.tie(0);
    int T;
    cin >> T;
    cout << divisors(T);
    for(int Q = 0; Q < T; Q++){
        int n, q, k, x;
        cin >> n >> q;
        int cur = n, d = divisors(n);
        for(int i = 0; i < q; i++){
            cin >> k;
            if(k == 2){
                cur = n;
                continue;
            }
            cin >> x;
            cur *= x;

        }
    }
}
