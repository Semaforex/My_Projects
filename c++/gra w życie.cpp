#include <iostream>
#include <windows.h>
#include <graphics.h>
using namespace std;

DWORD screen_width = GetSystemMetrics(SM_CXSCREEN);
DWORD screen_height = GetSystemMetrics(SM_CYSCREEN);
char t[102][102], t2[102][102];
int h=1;
int v=30;
POINT cursorPosition;
void setup(int a, int b, pair<int, int> z[], int n){
    initwindow(screen_width, screen_height);
    setcolor(15);
    for(int i = 1; i < b+1; i++){
        for(int k = 1; k < a+1; k++){
            rectangle(v*(k-1), v*(i-1), v*k, v*i);
        }
    }
    for(int i = 0; i < b+2; i++){
        for(int k = 0; k < a+2; k++){
            t[i][k] = '-';
            t2[i][k] = '-';
        }
    }
    for (int i = 0; i < n; i++) t[z[i].second][z[i].first] = 'o';
}

void logic(int a, int b){
    int suma;
    if(h == 0){
        for(int i = 1; i < b+1; i++){
            for(int k = 1; k < a+1; k++){
                suma=0;
                if(t[i][k+1] == 'o') suma++;
                if(t[i][k-1] == 'o') suma++;
                if(t[i+1][k] == 'o') suma++;
                if(t[i-1][k] == 'o') suma++;
                if(t[i+1][k+1] == 'o') suma++;
                if(t[i-1][k-1] == 'o') suma++;
                if(t[i+1][k-1] == 'o') suma++;
                if(t[i-1][k+1] == 'o') suma++;
                if(t[i][k] == '-'){
                    if(suma == 3){
                        t2[i][k] = 'o';
                    }
                    else t2[i][k] = '-';
                }
                else{
                    if(suma != 2 && suma != 3){
                        t2[i][k] = '-';
                    }
                    else{
                        t2[i][k] = 'o';
                    }
                }
            }
        }
        h++;
    }
    else{
        for(int i = 1; i < b+1; i++){
            for(int k = 1; k < a+1; k++){
                suma=0;
                if(t2[i][k+1] == 'o') suma++;
                if(t2[i][k-1] == 'o') suma++;
                if(t2[i+1][k] == 'o') suma++;
                if(t2[i-1][k] == 'o') suma++;
                if(t2[i+1][k+1] == 'o') suma++;
                if(t2[i-1][k-1] == 'o') suma++;
                if(t2[i+1][k-1] == 'o') suma++;
                if(t2[i-1][k+1] == 'o') suma++;
                if(t2[i][k] == '-'){
                    if(suma == 3){
                        t[i][k] = 'o';
                    }
                    else t[i][k] = '-';
                }
                else{
                    if(suma != 2 && suma != 3){
                        t[i][k] = '-';
                    }
                    else{
                        t[i][k] = 'o';
                    }
                }
            }
        }
        h--;
    }
}

void draw(int a, int b){

    if(h == 0){
        for(int i = 1; i < b+1; i++){
            for(int k = 1; k < a+1; k++){
                if(t2[i][k]=='o')
                    setcolor(15);
                else
                    setcolor(0);
                circle((k-1)*v+v/2, (i-1)*v+v/2, v/2);
                cout << t2[i][k];
            }
          cout << endl;
        }
    }
    else{
         for(int i = 1; i < b+1; i++){
            for(int k = 1; k < a+1; k++){
                if(t[i][k]=='o')
                    setcolor(15);
                else
                    setcolor(0);
                circle((k-1)*v+v/2, (i-1)*v+v/2, v/2);
                cout << t[i][k];
            }
            cout << endl;
        }
    }
}

int main(){
    GetCursorPos(&cursorPosition);
    int a, b, n, p;
    cout << "Wprowadz wymiary planszy (max. 100x100)" << endl;
    cin >> a >> b;
    cout << "wprowadz liczbe krokow" << endl;
    cin >> p;
    cout << "wprowadz liczbe poczatkowych komorek" << endl;
    cin >> n;
    cout << "wprowadz ich wspolzedne (liczone od lewego gornego rogu)" << endl;
    pair<int, int> z[n];
    for(int i = 0; i < n; i++) cin >> z[i].first >> z[i].second;
    setup(a, b, z, n);
    draw(a, b);
    h = 0;
    cout << "start?" << endl;
    cin >> n;
    for(int i = 0; i < p; i++)
        logic(a, b);
        draw(a, b);
        cout << endl;
        cout << endl;
        Sleep(800);
    }
