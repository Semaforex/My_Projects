#include<iostream>
#include<vector>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    long long n;
    cin >> n;
    vector<long long> w = {n};
    while(n>1){
        if(n%2) n = 3*n+1;
        else n /= 2;
        w.push_back(n);
    }
    for(int i = 0; i < w.size(); i++) cout << w[i] << ' ';
}
