#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int Q = 0; Q < T; Q++){
        long long x, y;
        cin >> y >> x;
        x--;
        y--;
        if(x > y)
            cout << (x/2*2+1)*(x/2*2+1)+(x%2)+((x%2)*2-1)*y;
        if(y >= x)
            cout << ((y+1)/2*2)*((y+1)/2*2)+((y+1)%2)+(((y+1)%2)*2-1)*x;
        cout << '\n';
    }
}
