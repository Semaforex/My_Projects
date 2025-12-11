#include<iostream>
using namespace std;
long long M = 1000*1000*1000+7;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    long long i = 1;
    while(n > 0){
        n--;
        i = (2*i)%M;
    }
    cout << i;
}
