#include<iostream>
#define TS 262144
using namespace std;
int t[2*TS];
void chTree(int pos, int n){
    pos += TS;
    t[pos] = n;
    while(pos > 1){
        pos /= 2;
        t[pos] = max(t[2*pos+1], t[2*pos]);
    }
}
int main(){
    for(int i = 1; i <= 1000; i++) cout << i << ' ';
}
