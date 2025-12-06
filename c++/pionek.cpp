#include <bits/stdc++.h>
using namespace std;
int NWD(int a, int b){
    int r;
    while (b!=0){
        r=a%b;
        a=b;
        b=r;
        }
    return a;
    }
int main(){
    int n, a, b, x1, y1, x2, y2;
    cin >> n;
    for (int i=0; i<n; i++){
        cin >> a >> b >> x1 >> y1 >> x2 >> y2;
        int dy=abs(y2-y1);
        int dx=abs(x2-x1);
        int nwd=NWD(a, b);
        int d=abs(dx-dy), r=a-b;
        if(r==0 && d!=0) cout << "NIE" << endl;
        else{
        if(r==0) r=1;
        if(dx%nwd==0 && dy%nwd==0 && d%r==0)
        cout << "TAK";
        else cout << "NIE"; cout << endl;
        }
        }
    }
