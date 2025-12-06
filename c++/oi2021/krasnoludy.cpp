#include<iostream>
using namespace std;

int t[100002], z[100002], n;
bool odw[100002], czy[100002], sas[100002];

int main(){
    cin >> n;
    odw[n+2] = 1;
    odw[0] = 1;
    for(int i = 2; i < n+2; i++)
        cin >> t[i];
    //na razie zakladam ze sie da
    for(int i = 3; i <= n; i++){
        if(t[i-1] == t[i+1]){
            odw[i]++;
            sas[i-1]++;
            sas[i+1]++;
            z[i] = t[i-1];
        }
    }
    /*if(!sas[2]){
        z[1] = t[2];
        sas[2]++;
    }
    if(!sas[n]){
        z[n+1] = t[n];
        sas[n]++;
    }
    sas[0]++;
    sas[n+2]++;*/
    z[2] = t[1];
    sas[1]++;
    z[n] = t[n+1];
    sas[n+1]++;
    sas[0]++;
    sas[n+2]++;
    for(int i = 1; i < n-1; i++){
            if(!odw[i]){
                int j = i;
                while(!sas[j+1]){
                    z[j] = t[j+1];
                    j+=2;

                }
                while(!sas[j-1]){
                    z[j] = t[j-1];
                    j-=2;
                }
            }
    }
    for(int i = 2; i <= n+1; i++) cout << z[i] << ' ';
}
