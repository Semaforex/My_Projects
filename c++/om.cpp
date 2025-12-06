#include <bits//stdc++.h>
using namespace std;
int main(){
    int n, p;
    bool x;
    cin >> n;
    for (int i = 0; i < n; i++){
        x = true;
        //if(int(cbrt(i))*int(cbrt(i))*int(cbrt(i)) < i){
       //     p = cbrt(i)+1;
       //     cout << "lolz" << '\n';
       // }
       // else p = cbrt(i);
        for (int y = ceil(sqrt(i)); y <= cbrt(i*i); y++){
            if(i%y == 0) x = false;
        }
        if(x){
            vector <int> dziel;
            for (int y = 1; y < cbrt(i); y++){
                if(i%y == 0)
                    dziel.push_back(y);
            for (int y = 0; y < dziel.size(); y++) cout << dziel[y] << ' ';
            for (int y = 0; y < dziel.size(); y++) cout << i/dziel[y] << ' ';
            cout << '\n';
            }
        }
    }
}
