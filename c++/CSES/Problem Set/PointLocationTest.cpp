#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int Q = 0; Q < T; Q++){
        long long x1, y1, x2, y2, x3, y3;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
        long long v = (x3-x1)*(y2-y1)-(y3-y1)*(x2-x1);
        if(v == 0) cout << "TOUCH" << '\n';
        if(v < 0) cout << "LEFT" << '\n';
        if(v > 0) cout << "RIGHT" << '\n';
    }
}
