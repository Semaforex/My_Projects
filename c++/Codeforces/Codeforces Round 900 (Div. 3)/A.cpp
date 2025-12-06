#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int Q = 0; Q < T; Q++){
        int n, k, x;
        bool p = false;
        cin >> n >> k;
        for(int i = 0; i < n; i++){
            cin >> x;
            if(x == k && !p){
                cout << "yes" << '\n';
                p = true;
            }
        }
        if(!p) cout << "no" << '\n';
    }
}
