#include<iostream>
#include<algorithm>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    //cin.tie(0);
    int q;
    cin >> q;
    for(int t = 0; t < q; t++){
        long long n, x, y;
        cin >> n >> x >> y;
        long long g = (x*y)/__gcd(x, y), c = n/g, X = n/x, Y = n/y;
        Y -= c;
        X -= c;
        cout << (n*(n+1)-(n-X)*(n-X+1))/2-Y*(Y+1)/2 << '\n';
    }
}
