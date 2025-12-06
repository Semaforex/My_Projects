#include <bits//stdc++.h>
using namespace std;

char t[1000000];
int zamien(int a, int b){
    for (int i=a; i<=b; i++){
        if(t[i] == 'x') t[i] = 'o';
        else t[i] = 'x';
    }
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, x, y;
    cin >> n >> m;
    char a;
    for (int i=1; i<=n; i++) cin >> t[i];
    for (int i=0; i<m; i++){
        cin >> a;
        if(a == 'Q'){
            cin >> x;
            if(t[x] == 'x') cout << "OFF";
            else cout << "ON";
            cout << endl;
        }
        else{
            cin >> x >> y;
            zamien(x, y);
        }
    }

}
