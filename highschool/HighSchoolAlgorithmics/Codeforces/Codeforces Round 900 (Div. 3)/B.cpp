#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int Q = 0; Q < T; Q++){
        int n;
        cin >> n;
        for(int i = 1; i < 3*n+1; i += 3)
            cout << i << ' ';
        cout << '\n';
    }
}
