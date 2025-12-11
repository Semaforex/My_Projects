#include<iostream>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int q;
    cin >> q;
    for(int t = 0; t < q; t++){
        int n, onecount=0, x;
        long long sum=0;
        cin >> n;
        if (n == 1){
            cin >> x;
            cout << "NO" << '\n';
            continue;
        }
        for(int i = 0; i < n; i++){
            cin >> x;
            if(x == 1) onecount++;
            else sum += x-2;
        }
        if(2*onecount > n){
            if(onecount-sum > n-onecount) cout << "NO" << '\n';
            else cout << "YES" << '\n';
        }
        else cout << "YES" << '\n';
    }
}
