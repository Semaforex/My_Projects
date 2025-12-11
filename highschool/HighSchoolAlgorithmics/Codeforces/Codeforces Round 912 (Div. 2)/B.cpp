#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int Q = 0; Q < T; Q++){
        int n;
        long long sum = 0;
        cin >> n;
        vector<int> v;
        int x;
        for(int i = 0; i < n; i++){
            cin >> x;
            if(x < 0) sum += x;
            else v.push_back(x);
        }
        sort(v.begin(), v.end());
        for(int i = 0; i < v.size(); i++) cout << v[i] << ' ';
        cout << '\n';
        for(int i = 0; i < v.size(); i++)
            sum += (i+2)*v[i];
        cout << sum << '\n';
    }
}
