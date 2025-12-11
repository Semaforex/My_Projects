#include <bits/stdc++.h>
using namespace std;
int main(){
    long long g1, d1, g2, d2, wg, wd, n;
    cin >> n;
    for (int i=0; i<n; i++){
        cin >> g1 >> d1 >> g2 >> d2;
        long long nww=d1*d2/__gcd(d1, d2);
        wg=nww/d1*g1+nww/d2*g2;
        wd=nww;
        long long y=__gcd(wg, wd);
        wg/=y;
        wd/=y;
        cout<< wg << " " << wd << endl;
    }
}
