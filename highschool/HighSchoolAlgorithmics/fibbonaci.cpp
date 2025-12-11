#include <bits//stdc++.h>
using namespace std;
int main(){
    float n;
    cin >> n;
    for (int i=1; i<=n; i++){
        int x=round(1/sqrt(5)*pow(((1+sqrt(5))/2), i)-1/sqrt(5)*pow(((1-sqrt(5))/2), i));
        for (int y=0; y<x; y++) cout << "*";
        cout << endl;
    }
}
