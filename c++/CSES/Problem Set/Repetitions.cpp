#include<iostream>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    string s;
    cin >> s;
    int c = 0, m = 1, i = 0;
    char cur = s[0];
    while(i+1 < s.size()){
        while(s[i] == cur && i+1 < s.size()){
            i++;
            c++;
        }
        if(s[i] == cur) c++;
        cur = s[i];
        m = max(m, c);
        c = 1;
        i++;
    }
    cout << m;
}
