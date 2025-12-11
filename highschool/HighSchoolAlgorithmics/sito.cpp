#include <iostream>
using namespace std;
int t[1299710];
int main(){
    ios_base::sync_with_stdio(0);
    long long n, x=0, k=2;
    cin >> n;
    for (int i=2; i*i<100*n; i++){
        for (int j=i*i; j<1299710; j+=i) t[j]=1;
    }
    while (x<n){
        if(t[k]==0){
            cout << k << " ";
            x++;
        }
        k++;
    }
}
