#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t, n, suma, maxim = 1000*1000*1000+7;
    cin >> t;
    for (int i = 0; i < t; i++){
        suma = 0;
        cin >> n;
        for (int y = 2; y < n; y++){
            if(n%y != 0) suma = (suma + y)%maxim;
        }
    cout << suma << endl;
    }
}
