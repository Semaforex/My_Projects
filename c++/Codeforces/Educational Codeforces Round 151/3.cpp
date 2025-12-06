#include<iostream>
#include<math.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    for(int q = 0; q < t; q++){
        int n;
        cin >> n;
        long long a[n+1], sum = 0, minsum = 0, pref[n+1];
        for(int i = 1; i <= n; i++) cin >> a[i];
        pref[0] = 0;
        for(int i = 1; i <= n; i++) pref[i] = pref[i-1]+a[i];
        long long ans = pref[n];
        for(int i = n; i > 0; i--){
            sum = sum+a[i];
            if(sum < minsum){
                minsum = sum;
                ans = pref[i-1];
            }
        }
        cout << ans << '\n';
    }
}
