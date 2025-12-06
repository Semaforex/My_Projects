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
    x = 1, y = 0;
    long long a, b, m, wyn;
    cin >> a >> b >> m;
    if(b%m == 0){
        cout << 0;
        return 0;
    }
    euklid(a, m);
    long long d = x*a+y*m;
    if(d == 1){
        wyn = (x*b)%m;
        while(wyn < 0) wyn += m;
        cout << wyn;
        return 0;
    }
    if(b%d){
        cout << "NIE";
        return 0;
    }
    wyn = x*(b/d);
    while(wyn > 0) wyn -= m/d;
    while(wyn < 0) wyn += m/d;
    cout << wyn;
    return 0;
}
