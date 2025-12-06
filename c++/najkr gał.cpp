#include <bits//stdc++.h>
using namespace std;
int main(){
int k;
long long n=0, sum, wyn=1000000000;
cin >> k;
for (int i=0; i<k; i++) n+=pow(2, i);
int t[n+1], p=k-1;
for (int i=1; i<=n; i++) cin >> t[i];
for (int i=n/2+1; i<=n; i++){
    sum=0;
    p=i;
    while(p>=1){
        sum +=t[p];
        p/=2;
    }
    wyn=min(wyn, sum);
}
cout << wyn;
}

