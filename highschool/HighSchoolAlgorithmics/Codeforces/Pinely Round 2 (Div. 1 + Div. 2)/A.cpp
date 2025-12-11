#include<iostream>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    for(int p = 0; p < t; p++){
        int n, b, cur, q, sp = 0;
        bool Sure = false;
        cin >> n >> b >> q;
        if(b == n)
            Sure = true;
        char x;
        cur = b;
        for(int i = 0; i < q; i++){
            cin >> x;
            if(Sure) continue;
            if(x == '+'){
                cur++;
                sp++;
                if(cur == n)
                    Sure = true;
            }
            else
                cur--;
        }
        if(Sure){
            cout << "YES" << '\n';
            continue;
        }
        if(sp+b >= n){
            cout << "MAYBE" << '\n';
            continue;
        }
        cout << "NO" << '\n';
    }
}
