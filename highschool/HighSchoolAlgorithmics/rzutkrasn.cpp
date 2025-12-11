#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, i=3, wynik=3, najw=4;
    cin >> n;
    n-=2;
    while (n-i>=0){
        n=n-i;
        wynik++;
        i++;
    }
    cout << wynik << endl;
    vector<int> wyniki[wynik+1];
    wyniki[1].push_back(1);
    wyniki[1].push_back(2);
    wyniki[2].push_back(1);
    wyniki[2].push_back(3);
    wyniki[3].push_back(2);
    wyniki[3].push_back(3);
    for (int i=4; i<=wynik; i++){
        for (int y=najw; y<=i+2; y++){
            wyniki[y-najw].push_back(najw);
            //wynik[i+3]
            najw++;
        }
    }

    for (int i=1; i<=wynik; i++){
        for (int y=0; y<wyniki[i].size(); y++) cout << wyniki[i][y] << " ";
        cout << endl;
    }
}
