#include <iostream>
using namespace std;

long long pot(int a, int b, int m)
{
long long i;
long long result = 1;
long long x = a%m;

for (i=1; i<=b; i<<=1)
{
x %= m;
if ((b&i) != 0)
{
result *= x;
result %= m;
}
x *= x;
}
return result;
}
int main(){
long long n, x, y;
cin >> n;
for (int i=0; i<n; i++){
    cin >> x >> y;
    cout << pot(x, y, 10000007) << endl;
}
}
