#include<iostream>
#include<math.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; i++){
        long long x1, x2, x3, y1, y2, y3;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
        x2 -= x1;
        x3 -= x1;
        y2 -= y1;
        y3 -= y1;
        if(x2*x3 < 0){
            if(y2*y3 < 0) cout << 1;
            else cout << min(abs(y2), abs(y3))+1;
        }
        else{
            if(y2*y3 < 0) cout << min(abs(x2), abs(x3))+1;
            else cout << min(abs(x2), abs(x3)) + min(abs(y2), abs(y3)) +1;
        }
        cout << '\n';
    }
}
