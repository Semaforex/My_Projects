#include <iostream>
using namespace std;
int main(){
    int n, w, s=0, x, k=0;
    cin >> n >> w;
    for (int i=0; i<n; i++){
        cin >> x;
        s+=x;
        if(s>=w){
            k++;
            s=0;
        }
    }
    cout << k;
}
