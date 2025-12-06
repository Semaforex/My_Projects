#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, h, k = 0, ile = 0;
    cin >> n >> h;
    int t[h+2], p, minim = n;
    for (int i = 0; i <= h+1; i++) t[i] = 0;
    for (int i = 0; i < n; i++){
        cin >> p;
        if(k == 0){
            k = 1;
            t[1] += 1;
            t[p+1] -= 1;
        }
        else{
            k = 0;
            t[h+1] -= 1;
            t[h-p+1] += 1;
        }
    }
    int ob = 0;
    for (int i = 1; i <= h; i++){
        if(ob+t[i] < minim){
            ile = 1;
            minim = ob+t[i];
        }
        else if(ob+t[i] == minim) ile++;
        ob += t[i];
    }
    cout << minim << " " << ile << '\n';
}
