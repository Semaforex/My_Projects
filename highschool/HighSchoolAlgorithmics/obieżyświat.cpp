#include <bits//stdc++.h>
using namespace std;
int main(){
    long long n, wyn = 1;
    cin >> n;
    long long max = 1000000007;
    for(int i = 2; i <= n; i++){
        wyn = (wyn*i)%max;
    }
    cout << wyn;
}
