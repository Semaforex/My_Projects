#include <iostream>
using namespace std;
int main(){
    cout << "Peter Griffin" << '\n';
    cout << "No licze NWD no" << '\n';
    int a, b, c;
    cin >> a >> b;
    while(b != 0){
        c = a%b;
        a = b;
        b = c;
    }
    cout << a;
}
