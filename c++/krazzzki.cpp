#include<iostream>
#include<algorithm>
using namespace std;
int main(){
int n,m;
cin>>n>>m;
int t[n];
int tab[m];
for(int i=0;i<n;i++){
cin>>t[i];
}
for(int i=1;i<n;i++){
if(t[i]>t[i-1]){
t[i]=t[i-1];
}
}

sort(t,t+n);
for(int i = 0; i < n; i++) cout << t[i] << ' ';
cout << endl;

for(int i=0;i<m;i++){
cin>>tab[i];
}
int z=n;
for(int i=0;i<m;i++){
    int poc=0,kon=z-1,wynik =-1,sr;
    while(poc<kon){
        sr=(poc+kon)/2;
        if(t[sr]>=tab[i]){
            kon=sr;
            wynik = sr;
                   }

                   else{
                    poc=sr+1;
                   }

    }
z = n - wynik;
}
cout << z;


}
