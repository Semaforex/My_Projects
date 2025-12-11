#include<bits//stdc++.h>
using namespace std;
long long x, y;
void euklid(int a, int b)
{
	if(b!=0)
	{
		euklid(b, a%b);
		int pom = y;
		y = x-a/b*y;
		x = pom;
	}
}
int main(){
    x = 1;
    y = 0;
    long long a, b, m, n;
    cin >> m >> n >> a >> b;
    euklid(m, n);
    long long w = x*(b-a)%n;
    if(w < 0) w += n;
    cout << w*m+a;
}
