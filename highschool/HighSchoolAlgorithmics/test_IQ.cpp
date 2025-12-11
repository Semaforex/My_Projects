#include <iostream>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    bool czy;
    int n, z, x, b;
    cin >> n;
    int t[n];
    for(int i = 0; i < n; i++)
        cin >> t[i];
    cin >> z;
    for(int i = 0; i < z; i++){
        cin >> x;
        int ob = 0;
        czy = true;
        for(int y = 0; y < x; y++){
            cin >> b;
            czy = false;
            for(int w = ob; w < n; w++){
                if(t[w] == b){
                    ob = w+1;
                    czy = true;
                    w = n;
                }
            }
        }
        if(czy) cout << "TAK" << endl;
        else cout << "NIE" << endl;
    }
}
