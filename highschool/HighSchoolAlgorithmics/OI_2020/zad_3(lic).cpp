#include <iostream>
using namespace std;
int W[100000], Z[100000], S[100000], p = 0;
void Wewn(int a, int b){
    p = b-W[a];
    W[a] = b;
    while(p != 0){
        int ob = S[a]+p;
    if(ob >= 0){
        S[a] = ob%10;
        p = ob/10;
    }
    else{
        S[a] = 10+ob;
        p = -1;
    }
    a--;
    }
}
void Zewn(int a, int b){
    p = b-Z[a];
    Z[a] = b;
    while(p != 0){
        int ob = S[a]+p;
    if(ob >= 0){
        S[a] = ob%10;
        p = ob/10;
    }
    else{
        S[a] = 10+ob;
        p = -1;
    }
    a--;
    }
}
int main(){
    int n, z, ob;
    string we, ze;
    cin >> n >> z;
    cin >> we >> ze;
    W[0] = 0;
    Z[0] = 0;
    for(int i = 1; i < n; i++){
        W[i] = int(we[i-1]-'0');
        Z[i] = int(ze[i-1]-'0');
    }
    for(int i = n-1; i >= 0; i--){
        ob = Z[i]+W[i]+p;
        S[i] = ob%10;
        p = ob/10;
    }
    int x, y;
    char a;
    for(int i = 0; i < z; i++){
        cin >> a;
        if(a == 'S'){
            cin >> x;
            cout << S[n-x] << '\n';
        }
        else if(a == 'W'){
            cin >> x >> y;
            Wewn(n-x, y);
        }
        else{
            cin >> x >> y;
            Zewn(n-x, y);
        }
    }
}
