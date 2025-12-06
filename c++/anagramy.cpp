#include <iostream>
using namespace std;
int t[26];
int main(){
    string n;
    cin >> n;
    for (int i=0; i<n.size(); i++) t[int(n[i])-97]++;
    cin >> n;
    for (int i=0; i<n.size(); i++) t[int(n[i])-97]--;
    for (int i=0; i<26; i++) if(t[i]!=0){
        cout << "NIE";
        return 0;
    }
    cout << "TAK";
}
