#include<iostream>
#include<cmath>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int q;
    cin >> q;
    for(int t = 0; t < q; t++){
        float a, b, c;
        cin >> a >> b >> c;
        cout << (ceil(abs(a-b)/(2*c))) << '\n';
    }

}
