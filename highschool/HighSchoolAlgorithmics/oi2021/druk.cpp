#include<iostream>
#include<cmath>
#include<vector>
#include<algorithm>
using namespace std;
int n, m;
char t[1000];
bool czy = true;
vector<int> v, wyn;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> m;
    if(n > 1){
        cout << "lol";
        return 0;
    }
    for(int i = 0; i < m; i++)
        cin >> t[i];
    for(int i = 2; i <= sqrt(m); i++){
        if(m%i == 0){
            v.push_back(i);
            if(m/i != i) v.push_back(m/i);
        }
    }
    v.push_back(1);
    wyn.push_back(m);
    for(int i = 0; i < v.size(); i++){
        for(int y = 0; y < v[i]; y++){
            for(int z = v[i]; z < m; z+=v[i]){
                if(t[z+y] != t[z+y-v[i]]) czy = false;
            }
        }
        if(czy) wyn.push_back(v[i]);
        czy = true;
    }
    sort(wyn.begin(), wyn.end());
    cout << wyn.size() << '\n';
    for(int i = 0; i < wyn.size(); i++)
        cout << wyn[i] << ' ';
    return 0;
}
