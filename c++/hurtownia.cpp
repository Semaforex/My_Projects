#include<bits//stdc++.h>
#define P2 262144
using namespace std;
int n;
long long pref[P2*2];
//uzupelnianie drzewa
void dod(int a, int poz){
    poz += P2;
    pref[poz] = a;
    poz /= 2;
    while(poz > 0){
        pref[poz] = pref[2*poz]+pref[2*poz+1];
        poz /= 2;
    }
}
//suma przedzialu od 0 do i
long long suma(int poz){
    poz += P2+1;
    long long w = 0;
    while(poz > 0){
        if(poz%2)
            w += pref[poz-1];
        poz /= 2;
    }
    return w;
}
//najdalszy niezerowy elemnt mniejszy od poz
int najnie0(int poz){
    long long sufix = suma(n)-suma(poz);
    poz = 1;
    while(poz < P2){
        poz *= 2;
        if(pref[poz+1] > sufix) poz++;
        else sufix -= pref[poz+1];
    }
    return poz;
}
//uzupelnianie sum prefixowych i naj
void rmTree(int pos, long long v){
    pos = najnie0(pos - P2);
    while(v){
        long long av = min(pref[pos], v);
        dod(pref[pos] - av, pos - P2);
        v -= av;
        pos = najnie0(pos - P2);
    }
}

int main(){
    int a, w = 0;
    cin >> n;
    pair<int, int> t[n];
    for(int i = 0; i < n; i++){
        cin >> a;
        dod(a, i);
    }
    for(int i = 0; i < n; i++){
        cin >> t[i].first;
        t[i].second = i;
    }
    sort(t, t+n);
    for(int i = 0; i < n; i++){
        if(suma(t[i].second) >= t[i].first){
            w++;
            if(t[i].first)
                rmTree(t[i].second+P2, t[i].first);
        }
    }
    cout << w;

}
