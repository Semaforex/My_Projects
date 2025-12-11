#include <iostream>
using namespace std;
int main(){
    int q = 1000000019, hasz = 0, p = 31;
    long long pods = 1;
    string s;
    cin >> s;
    for(int i = 0; i < s.size(); i++){
        hasz = (pods * int(s[i])aa + hasz) % q;
        pods = (pods * p) % q;
    }
    cout << hasz;
}
