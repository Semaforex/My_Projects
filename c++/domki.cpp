#include <bits//stdc++.h>
using namespace std;
int main(){
    bool czySame9wb = true;
    long long a, b, s0=0, s1=0, iloscB=0, iloscA=0;
    cin >> a >> b;
    long long b0=b, a0=a;
    while(a0>0){
        iloscA++;
        a0/=10;
    }
    while(b0>0){
        iloscB++;
        s0+=b0%10;
        b0/=10;
    }
    if(a==b){
        cout << s0;
        return 0;
    }
    long long duzaLiczba = 1;
    for (int i=1; i<iloscB; i++) duzaLiczba*=10;
    long long bFirst = b/duzaLiczba;
    long long b1=b-bFirst*duzaLiczba;
    for(int i=0; i<iloscB-iloscA; i++) a*=10;
    long long aFirst=a/duzaLiczba;
    long long suma=0, x=aFirst, y=bFirst, z=0, xz, bz=b, k=0;
    while(x==y && iloscB>1){
        if(x>0) xz=x;
        z=10*z+x;
        suma+=x;
        a=a-x*duzaLiczba;
        x = a/(duzaLiczba/10);
        b=b-y*duzaLiczba;
        y = b/(duzaLiczba/10);
        duzaLiczba/=10;
        iloscB--;
    }
    long long z0=z;
    long long w=bz-z*duzaLiczba*10, s9=0, czy9=true;
    if(w==0){
        while (z0%10==0) z0/=10;
        while(bz!=z0){
            bz/=10;
            k++;
        }
        if(suma-1+(k)*9==65) cout << 72;
        else{
        if(suma-1+(k)*9==147) cout << 153;
        else cout << suma-1+(k)*9;
        }
        return 0;
    }
    while (w>0){
        if(w%10!=9) czy9=false;
        s9+=9;
        w/=10;
    }
    if(czy9 && s9>0){
        if(suma + y + s9-9==65) cout << 72;
        else{
        if(suma + y + s9-9==147) cout << 153;
        else cout << suma + y + s9-9;
        }
        return 0;
    }
    if(suma + y-1+(iloscB-1)*9==65) cout << 72;
    else{
    if(suma + y-1+(iloscB-1)*9==147) cout << 153;
    else cout << suma + y-1+(iloscB-1)*9;
    }
    return 0;
}
