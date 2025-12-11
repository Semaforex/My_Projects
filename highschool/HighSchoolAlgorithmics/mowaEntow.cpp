#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    char dzwieczne[16] = {'a', 'e', 'i', 'o', 'u', 'y', 'b', 'd', 'g', 'j', 'l', 'm', 'n', 'r', 'w', 'z'};
    char bezdzwiaczne[7] = {'c', 'f', 'h', 'k', 'p', 's', 't'};
    int n, wynikob, wynik = -1;
    cin >> n;
    string s, swynik;
    for (int h = 0; h < n; h++){
        cin >> s;
        int prefdz[s.size()+1];
        int prefbez[s.size()+1];
        prefdz[0] = 0;
        prefbez[0] = 0;
        for(int i = 1; i <= s.size(); i++){
            prefbez[i] = prefbez[i-1];
            prefdz[i] = prefdz[i-1]+1;
            for (int y = 0; y < 7; y++){
                if(s[i-1] == bezdzwiaczne[y]){
                    prefbez[i] = prefbez[i-1]+1;
                    prefdz[i] = prefdz[i-1];
                    break;
                }
            }
        }
            wynikob = 0;
            for(int pocz = 1; pocz <= s.size(); pocz++){
                for(int kon = pocz+wynik; kon <=s.size(); kon++){
                    if(prefbez[kon]-prefbez[pocz-1] == prefdz[kon] - prefdz[pocz-1]){
                        wynikob = max(wynikob, kon - pocz+1);
                    }
                }
            }
            if(wynikob >= wynik){
                wynik = wynikob;
                swynik = s;
            }
        }
    cout << swynik << " " << wynik;
}
