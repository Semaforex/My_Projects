#include <bits/stdc++.h>
using namespace std;
long long p[19] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67};
long long odp = 100000000000000;
void factor(int n, int naj, vector<long long> v){
    if(n == 1){
        long long suma = 1;
        for(int i = 0; i < v.size(); i++){
            suma *= pow(p[i],(v[i]-1));
        }
        if(suma < 0) return;
        odp = min(odp, suma);
        return;
    }
    for(int i = naj; i > 1; i--){
        if(n%i == 0){
            vector<long long> a = v;
            a.push_back(i);
            factor(n/i, i, a);
        }
    }
}
int main()
{
    int n;
    cin >> n;
    vector<long long> p;
    factor(n, n, p);
    cout << odp;
    return 0;
}
