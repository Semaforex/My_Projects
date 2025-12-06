#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    if((n/2+(n%2))%2){
        cout << "NO";
        return 0;
    }
    cout << "YES" << '\n';
    if(!(n%4)){
        cout << n/2 << '\n';
        for(int i = 1; i <= n; i+=4)
            cout << i << ' ' << i+3 << ' ';
        cout << '\n' << n/2 << '\n';
        for(int i = 1; i <= n; i+=4)
            cout << i+1 << ' ' << i+2 << ' ';
    }
    else{
        cout << n/2+1 << '\n' << "1 2 ";
        for(int i = 4; i <= n; i+=4)
            cout << i << ' ' << i+3 << ' ';
        cout << '\n' << n/2 << '\n' << "3 ";
        for(int i = 4; i <= n; i+=4)
            cout << i+1 << ' ' << i+2 << ' ';
    }
}
