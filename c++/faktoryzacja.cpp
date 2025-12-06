#include <iostream>
#include <cmath>

using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    for (int y = 0; y < n; y++){
        int d, i, g;
        cin >> d;
        g = (int)sqrt(d);
        for(i = 2; i <= g; i++){
            while(!(d%i)){
                    cout << i << " ";
                    d /= i;
                }
            if(d == 1) break;
        }
        if(d > 1) cout << d;
        cout << '\n';
    }
    return 0;
}
