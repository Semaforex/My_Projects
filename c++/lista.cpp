#include <iostream>
using namespace std;
pair<int, int> t[200001];
int main(){
    char a;
    int b, c;
    int n, pierw = -1;
    cin >> n;
    for(int i = 1; i <= n; i++){
        cin >> a;
        if(a == '<')
            pierw = t[pierw].second;
        if(a == '>'){
            cin >> b;
            t[i].first = b;
            t[i].second = pierw;
            pierw = i;
        }
        if(a == '-'){
            cin >> b;
            t[b].first = -1;
        }
        if(a == '+'){
            cin >> b >> c;
            int x = t[c].second;
            t[c].second = i;
            t[i].first = b;
            t[i].second = x;
        }
    }
    int y = pierw;
    while(t[y].second != -1){
        if(t[y].first != -1) cout << t[y].first << ' ';
        y = t[y].second;
    }
    if(t[y].first != -1) cout << t[y].first;
}
