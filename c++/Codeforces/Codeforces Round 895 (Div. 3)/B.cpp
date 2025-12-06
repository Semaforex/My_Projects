#include<iostream>
#include<cmath>
#include<algorithm>
using namespace std;
pair<int, int> t[101];
bool check(int k, int n){
    int i = 1;
    while(t[i].first < k & i <= n){
        int s = t[i].second, d = t[i].first;
        if(s <= 2*(k-d)) return false;
        i++;
    }
    return true;
}
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int q;
    cin >> q;
    for(int z = 0; z < q; z++){
        int n;
        cin >> n;
        for(int i = 1; i <= n; i++)
            cin >> t[i].first >> t[i].second;
        sort(t+1, t+n+1);
        int maxi = t[n].first+t[n].second/2+2;
        while(true){
            if(check(maxi, n)){
                cout << maxi << '\n';
                break;
            }
            maxi--;
        }
    }

}
