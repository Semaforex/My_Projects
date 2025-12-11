#include <iostream>
using namespace std;
int main(){
    int n, k, l, ob = 0;
    cin >> n >> k >> l;
    int t[n], tk[n], tl[n];
    for(int i = 0; i < n; i++) cin >> t[n];
    for(int i = 0; i < k; i++) ob += t[i];
    t[k-1] = ob;
    for(int i = k; i < n; i++){
        t[i] = max(ob-t[i-k]+t[i], tk[i-1]);
        ob = ob-t[i-k]+t[i];
        cout << tk[i] << ' ';
    }
}
