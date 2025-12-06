#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    long long n, s=0, so=0, x;
    cin >> n;
    for (int i=0; i<n; i++){
        cin >> x;
        if(x==0) so++;
        else{
            s+=so;
        }
    }
    cout << s;
}
