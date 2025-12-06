#include <iostream>
using namespace std;
int w[1000001], ost[1000001], pocz[1000001], w2[1000001];
int main(){
    int n, il=0, obec=0, curr;
    cin >> n;
    int t[n];
    for (int i=0; i<n; i++){
        cin >> t[i];
        if(w[t[i]]==0){
            il++;
            w[t[i]]++;
        }
        if(pocz[t[i]]==0) pocz[t[i]]=i+1;
        else ost[t[i]]=i+1;
    }
    for (int i=0; i<n; i++){
        if(pocz[t[i]]!=i+1 && ost[t[i]]!=i+1) t[i]=0;
    }
    for (int i=0; i<n; i++){
        if(t[i]!=0){
            if(w2[t[i]]==0){
                w2[t[i]]=1;
                obec++;
                if(obec==0) curr=t[i];
            }
            else{
                w2[t[i]]--;
                obec--;
            }
            if(obec>1){
                if(ost[curr]>ost[t[i]]){
                    t[ost[curr]-1]==0;
                    curr=t[i];
                }
                else t[i]==0;
                obec--;
                il--;

            }
        }
        cout << t[i];
    }
    cout << il;

}
