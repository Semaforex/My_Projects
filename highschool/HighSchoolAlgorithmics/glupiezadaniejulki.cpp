#include<iostream>
using namespace std;
int t[7]; //deklaracja tablicy t
void info(){  //funkcja void info wypisuje dane i tyle
    cout << "Imie Nazwisko" << '\n' << "obliczam sume dodatnich elementow 7-elementowej tablicy 't'" << '\n';
}
void wczytaj(){ //fukcja void wczytaj wczytuje dane do elementów wy¿ej zadeklarowanej tablicy
    for(int i = 0; i < 7; i++)
        cin >> t[i];
}
void wypisz(){ //fukcja void wypisz wypisuje elementy tablicy t
    for(int i = 0; i < 7; i++)
        cout << t[i] << ' ';
    cout << '\n';
}
int suma_dod(){ // funckja int suma oblicza i zwraca szukan¹ sumê
    int s = 0;
    for(int i = 0; i < 7; i++)
        if(t[i] > 0)
            s += t[i];
    return s;
}
int main(){ //no i standardowy main
    info();
    wczytaj();
    wypisz();
    cout << suma_dod();
}
