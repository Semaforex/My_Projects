
#include <bits//stdc++.h>
using namespace std;
void przetlumacz(int k, string s, int n){
    for (int i = 0; i < n; i++){
        if(int(s[i]) != 32){
            if(int(s[i]) - k < 65) s[i] = 'Z' - char(64+k-int(s[i]));
            else s[i] = s[i] - char(k);
        }
    }
    cout << s << endl;
}
int ilosc[26];
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, q, wynik = 0;
    cin >> n;
    vector<int> wyniki;
    cin.ignore();
    string s;
    getline(cin, s);
    for (int i = 0; i < s.size(); i++){
        if(int(s[i]) != 32) ilosc[int(s[i])-int('A')]++;
    }
    cin >> q;
    pair<char, int> t[q];
    for (int i = 0; i < q; i++){
        cin >> t[i].first >> t[i].second;
    }
    for (int i = 0; i < 26; i++){
        bool blad = false;
        for (int y = 0; y < q; y++){
            if(t[y].second != ilosc[(int(t[y].first)-65+i)%26]) blad = true;
        }
        if(!blad){
            wynik++;
            wyniki.push_back(i);
        }
    }
    cout << wynik << endl;
    for (int i = 0; i < wynik; i++) przetlumacz(wyniki[i], s, n);
}
