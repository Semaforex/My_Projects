#include<iostream>
#include<cmath>
using namespace std;
int isPrime(int num) {
        if (num <= 3)  return -1;
        if (num % 2 == 0) return 2;
        int range = sqrt(num);
        if (num % 3 == 0)
            return 3;
        for (int i = 5; i <= range; i += 6){
            if (num % i == 0)
                return i;
            if (num % (i + 2) == 0)
                return i+2;
        }

        return -1;
    }
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int q;
    cin >> q;
    for(int t = 0; t < q; t++){
        int l, r;
        cin >> l >> r;
        if(r < 4){
            cout << -1 << '\n';
            continue;
        }
        l = max(4, l);
        if(l == r){
            int p = isPrime(l);
            int m = l/p;
            if(m < 0) cout << -1 << '\n';
            else cout << p*(m-1) << ' ' << p << '\n';
            continue;
        }
        if(l%2 == 1) l++;
        int m = l/2;
        cout <<2*(m-1) << ' ' << 2 << '\n';
    }
}
