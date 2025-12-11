#include<iostream>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    for(int p = 0; p < t; p++){
        int n, k;
        cin >> n >> k;
        int t[n+1];
        bool w[n+1];
        for(int i = 0; i <= n; i++)
            w[i] = 0;
        for(int i = 0; i < n; i++){
            cin >> t[i];
            w[t[i]] = true;
        }
        for(int i = 0; i <= n; i++){
            if(!w[i]){
                t[n] = i;
                break;
            }
        }
        k = k%(n+1);
        for(int i = 0; i < n; i++)
            cout << t[(n+1+i-k)%(n+1)] << ' ';
        cout << '\n';
    }
}
