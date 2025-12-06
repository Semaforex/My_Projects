#include <bits//stdc++.h>
using namespace std;
int main(){
    sync_with_stdio(false);
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> t[n*(k+1)+1];
    for (int i=0; i<m; i++){
        int a, b;
        cin >> a >> b;
        t[a].push_back(b);
        for (int j=1; j<=k; j++){
            t[j*n+a].push_back(j*n+b);
            t[(j-1)*n+b].push_back(j*n+a);
        }
    }

    //BFS
    int ob;
    bool odw[n*(k+1)+1];
    for (int i=1; i<=n*(k+1); i++) odw[i]=false;
    int odl[n*(k+1)+1];
    odl[1]=0;
    vector<int> v;
    v.push_back(1);
    for (int i=0; i<v.size(); i++){

        ob = v[i];
        for(int j=0; j<t[ob].size(); j++){
            if(t[ob][j]%n==0){
                cout << odl[ob]+1;
                return 0;
            }
            if(odw[t[ob][j]]==false){
                odl[t[ob][j]]=odl[ob]+1;
                v.push_back(t[ob][j]);
                odw[t[ob][j]]==true;
            }
        }
    }


}
/*
5 6 1
1 2
2 3
3 4
4 5
3 1
5 2

        for (int i=1; i<=n*(k+1)+1; i++){
            cout << i << ": ";
            for (int j=0; j<t[i].size(); j++) cout << t[i][j] << ' ';
        cout << endl;
        }
*/
