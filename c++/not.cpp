#include <bits//stdc++.h>
using namespace std;
int main()
{
ios_base::sync_with_stdio(0);
cin.tie(0);
cout.tie(0);
int n, cz1, licz=0;
cin >> n;
int t[n], tab[n], Q, x, w;
cin >> t[0];
tab[0]=0;
for (int i=1; i<n; i++){
    cin >> t[i];
    if(t[i]>t[i-1])tab[i]=1;
    else tab[i]=0;
    //cout << tab[i];
}
cin >> Q;
for (int y=0; y<Q; y++){
    cin >> x;
    if(x==1) cout << n << endl;
    else{
        w=0;
        cz1=0;
        for (int i=0; i<n; i++){
            if(tab[i]==1){
                licz++;
                cz1=1;
            }
            else if(cz1==1){
                    if(licz+1>=x)
                    w+=licz-x+2;
                    cz1=0;
                    licz=0;
               //     cout << w << ' ' << i << endl;
            }
            if(i==n-1 && licz+1>=x){
                w+=licz-x+2;
                licz=0;
               // cout << w << ' ' << i << endl;
            }
        }
        cout << w << endl;
    }
}
}
