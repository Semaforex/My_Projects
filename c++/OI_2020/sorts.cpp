#include <bits//stdc++.h>
using namespace std;
int n;

//sortowanie przez wstawianie
int * insertSort(int t[]){
    for(int i = 1; i < n; i++){
        int y = i;
        while(t[y] < t[y-1] && y > 0){
            swap(t[y], t[y-1]);
            y--;
        }
    }
    return t;
}

//sortowanie przez wybór
char * selectionSort(char t[]){
    for(int i = 0; i < n; i++){
        char minim = t[i];
        int ob = i;
        for(int y = i+1; y < n; y++){
            minim = min(minim, t[y]);
            if(minim == t[y]) ob = y;
        }
        swap(t[ob], t[i]);
    }
    return t;
}

//Sortowanie babelkowe
pair<int, int> * bubbleSort(pair<int, int> t[]) {
    bool czy = true;
    while (czy){
        czy = false;
        for(int i = 0; i < n-1; i++)
            if(t[i] > t[i+1]){
                swap(t[i], t[i+1]);
                cout << '0' << ' ';
                czy = true;
            }
    }
    return t;
}

int main () {
    cin >> n;
    int t[n];
    char k[n];
    pair<int, int> l[n];
    for(int i = 0; i < n; i++) cin >> t[i];
    for(int i = 0; i < n; i++) cin >> k[i];
    for(int i = 0; i < n; i++) cin >> l[i].first >> l[i].second;
    int * pos1 = insertSort(t);
    char * pos2 = selectionSort(k);
    pair<int, int> * pos3 = bubbleSort(l);
    for(int i = 0; i < n; i++) cout << pos1[i] << ' ';
    cout << endl;
    for(int i = 0; i < n; i++) cout << pos2[i] << ' ';
    cout << endl;
    for(int i = 0; i < n; i++) cout << '(' << pos3[i].first << ' ' << pos3[i].second << ") ";
    return 0;
}
