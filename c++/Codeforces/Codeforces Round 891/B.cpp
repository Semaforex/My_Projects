#include<bits//stdc++.h>
using namespace std;
int t[2*100000+1];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int b;
    cin >> b;
    for(int q = 0; q < b; q++){
        string s;
        cin >> s;
        int L = s.size();
        int k = L+1;
        for(int i = 1; i <= L; i++)
            t[i] = int(s[i-1])-int('0');
        for(int i = L; i > 0; i--){
            if(t[i] < 5) continue;
            while(t[i-1] == 9) i--;
            t[i-1]++;
            k = i;
        }
        if(t[0]) cout << t[0];
        for(int i = 1; i < k; i++) cout << t[i];
        for(int i = k; i <= L; i++) cout << '0';
        cout << '\n';
        t[0] = 0;
    }

}
