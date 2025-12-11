#include<iostream>
#define C 1000000007
using namespace std;
struct kras{
    int sas;
    bool czys;
    int co;
};
bool poj[100001];
int wyst[100001];
kras t[100000];
int n;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> t[i].sas;
        poj[t[i].sas] = true;
    }
    if(n == 3 && t[0].sas != t[2].sas){
        cout << 0;
        return 0;
    }
    for(int i = 1; i < n-1; i++){
        if(t[i-1].sas == t[i+1].sas){
            t[i].co = t[i-1].sas;
            t[i+1].czys = true;
            t[i-1].czys = true;
        }
    }
    t[0].czys = true;
    t[n-1].czys = true;
    t[1].co = t[0].sas;
    t[n-2].co = t[n-1].sas;
    for(int i = 2; i < n-2; i++){
        if(!t[i].co){
            if(!t[i-1].czys && t[i-2].co > 0){
                t[i].co = t[i-1].sas;
                t[i-1].czys = true;
            }
        }
    }
    for(int i = n-3; i > 1; i--){
        if(!t[i].co){
            if(!t[i+1].czys && t[i+2].co > 0){
                t[i].co = t[i+1].sas;
                t[i+1].czys = true;
            }
        }
    }
    if(t[1].sas != t[2].co && t[2].co != 0){
        t[0].co = t[1].sas;
        t[1].czys = true;
    }
    if(t[n-2].sas != t[n-3].co && t[n-3].co != 0){
        t[n-1].co = t[n-2].sas;
        t[n-2].czys = true;
    }
    for(int i = 0; i < n; i++){
        wyst[t[i].co]++;
    }
    for(int i = 1; i <= n; i++){
        if(wyst[i] > 1){
            cout << 0;
            return 0;
        }
    }
    for(int i = 1; i < n-1; i++){
        if(!t[i].czys && t[i-1].co != 0 && t[i+1].co != 0){
            cout << 0;
            return 0;
        }
    }
    int i = 1, ile = 0;
    long long w = 1;
    for(int i = 1; i <= n; i++){
        if(!poj[i]){
            ile++;
            w = (w*ile)%C;
        }
    }
    while(i < n-1){
        int k = 1;
        while(i < n-1 && !t[i].czys){
            k++;
            i +=2;
        }
        w = (w*k)%C;
        i += 2;
    }
    i = 2;
    while(i < n-1){
        int k = 1;
        while(i < n-1 && !t[i].czys){
            k++;
            i +=2;
        }
        w = (w*k)%C;
        i += 2;
    }
    cout << w;
    return 0;
}
