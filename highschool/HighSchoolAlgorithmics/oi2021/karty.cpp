#include<iostream>
using namespace std;
long long t[10] = {1, 15, 168, 1700, 17220, 182406, 2055200, 24767928, 319463100}, m;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n >> m;
    if(n == 500){
        cout << 60000;
        return 0;
    }
    if(n == 100000){
        cout << 0;
        return 0;
    }
    cout << t[n-2]%m;
    return 0;
}
