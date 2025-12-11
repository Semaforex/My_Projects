#include <iostream>
using namespace std;
int obj(int a, int h){
    return a*a*h;
}
int main(){
    cout << "Julia Talmont-Kaminska" << '\n';
    cout << "obliczam objêtosc graniastoslupa prawidlowego czworokatnego na podstawie dlugoœci krawedzi podstawy i wysokosci" << '\n';
    int a, h;
    cin >> a >> h;
    cout << obj(a, h) << '\n';
}
