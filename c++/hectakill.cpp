#include <iostream>
using namespace std;
int main(){
    for(int i = 20; i > 0; i--){
        for(int y = 0; y < 20-i; y++) cout << ' ';
        for(int y = 0; y < i; y++) cout << '|';
        cout << "             ";
        for(int y = 0; y < i; y++) cout << '|';
        cout << endl;
    }
}
