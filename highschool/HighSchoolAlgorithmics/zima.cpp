#include <bits//stdc++.h>
using namespace std;
bool odw[1000000];
int srednica = -100000000;
int sr(vector<pair<int, int>> t[], int ob){
    odw[ob] = true;
    vector<int> v;
    for(int i = 0; i < t[ob].size(); i++){
        int sas = t[ob][i].first;
        if(!odw[sas]){
            if(t[ob][i].second%2 == 0)
                v.push_back(sr(t, sas)-1);
            else
                v.push_back(sr(t, sas)+1);
        }
    }
    if(v.size() > 0){
        sort(v.begin(), v.end());
        srednica = max(srednica, v[v.size()-1]+v[v.size()-2]);
        return v[v.size()-1];
    }
    else return 0;
}
int main(){
    for(int i = 0; i < 1000000; i++) odw[i] = false;
    int n, x, y, z, parz = 0, nparz = 0, suma;
    cin >> n;
    vector<pair<int, int>> t[n+1];
    for(int i = 1; i < n; i++){
        cin >> x >> y >> z;
        t[x].push_back(make_pair(y, z));
        t[y].push_back(make_pair(x, z));
        if(z%2 == 0) parz++;
        else nparz++;
        suma += z;
    }
    cout << sr(t, 1) << ' ' << srednica;
}
/*1 2 2
  2 3 1
  2 4 2
  4 5 1
  4 6 2
  6 7 1*/
