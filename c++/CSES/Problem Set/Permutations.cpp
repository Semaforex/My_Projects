#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    if(n == 2 or n == 3){
        cout << "NO SOLUTION";
        return 0;
    }
    if(n == 4){
        cout << "3 1 4 2";
        return 0;
    }
    int t[n], cur = 1;
    for(int i = 0; i < n; i += 2){
        t[i] = cur;
        cur++;
    }
    for(int i = 1; i < n; i += 2){
        t[i] = cur;
        cur++;
    }
    for(int i = 0; i < n; i++)
        cout << t[i] << ' ';
}
