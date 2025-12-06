#include <bits//stdc++.h>
using namespace std;

int n;
int a;
int w;
int x;
int y;
int dodaj;
int odejmij;
bool d;


int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    cin >> n >> a;
    int t[n+1];
    vector<int> wynik;
    for(int i = 1;i < n+1;i++){
        cin >> w;
        t[i] = w-a;
        if(t[i] > 0){
            dodaj = dodaj+t[i];
        }
        if(t[i] < 0){
            odejmij = odejmij+t[i];
        }
        if(t[i] == 0){
            d = 1;
        }
    }

    if(d == 1){
        cout << "TAK" << endl << "1" << endl << a;
    }
    else{
        bool b[n+1][dodaj-odejmij+1];
        for(int i = 0;i < n+1;i++){
            for(int j = 0;j < dodaj-odejmij+1;j++){
                b[i][j] = 0;
            }
        }

        for(int i = 1;i < n+1;i++){
            for(int j = 0;j < dodaj-odejmij+1;j++){
                if(j-t[i] >= 0&&j-t[i] <= dodaj-odejmij){
                    if(b[i-1][j-t[i]] == 1){
                        b[i][j] = 1;
                    }
                    else{
                        b[i][j] = b[i-1][j];
                    }
                }
                else{
                    b[i][j] = b[i-1][j];
                }
            }
            b[i][t[i]-odejmij] = 1;
        }

        if(b[n][-odejmij] == 1){
            x = n;
            y = -odejmij;
            while(x > 0 && y > 0){
                if(b[x][y] != b[x-1][y]){
                    wynik.push_back(t[x]+a);
                    y = y-t[x];
                }
                x--;
            }

            cout << "TAK" << endl << wynik.size() << endl;
            for(int i = 0; i < wynik.size(); i++){
                cout << wynik[i] <<" ";
            }
        }
        else{
            cout << "NIE";
        }
    }
}
