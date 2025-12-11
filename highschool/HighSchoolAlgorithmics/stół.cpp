#include <bits//stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, wyn=0;
    cin >> n;
    int t[n+1], od[n+1], x=n;
    for (int i=1; i<=n; i++){
        cin >> t[i];
        od[i]=0;
    }
    while (x>0){
        for (int i=1; i<=n; i++){
            if(od[i]==0){
                wyn++;
                od[i]=wyn;
                x--;
                int ob=t[i];
                while(od[ob]==0){
                    od[ob]=wyn;
                    ob=t[ob];
                    x--;
                }
            }
        }
    }
    cout << wyn;
}
