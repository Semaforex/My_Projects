#include <bits//stdc++.h>
using namespace std;
int fib(int n){
    if(n>2){
        return (fib(n-2)+fib(n-1))%10;
        n--;
    }
    else return n;
}
int main(){
    int n;
    cin >> n;
    int x=n/60, y=(n-2)/60;
    cout << fib(n-x*60)+fib(n-2-y*60);
}
