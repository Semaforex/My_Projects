#include <bits//stdc++.h>
using namespace std;
int main(){
    char a;
    int n, x;
    cin >> n;
    queue<int> q;
    for(int i = 0; i < n; i++){
        cin >> a;
        if(a == '-'){
            cout << q.front() << '\n';
            q.pop();
        }
        else{
            cin >> x;
            q.push(x);
        }
    }
}
