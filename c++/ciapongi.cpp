#include <bits//stdc++.h>
using namespace std;
int n, w2;
bool czys[200001];
long long dp1[200001], dp2[200001], naj[200001], suma, odl[200001], wyn;
vector<pair<int, long long>> V[200001];
vector<int> v[200001];
int wagi(int ob, int oj){
    long long sum = 1;
    for(int i = 0; i < v[ob].size(); i++)
        if(v[ob][i] != oj)
            sum += wagi(v[ob][i], ob);
    if(ob != oj){
        V[ob].push_back({oj, sum*(n-sum)});
        V[oj].push_back({ob, sum*(n-sum)});
    }
    suma += sum*(n-sum);
    return sum;
}
void DFS(int ob, long long d, int oj){
    odl[ob] = d;
    for(int i = 0; i < v[ob].size(); i++)
        if(V[ob][i].first != oj)
            DFS(V[ob][i].first, d+V[ob][i].second, ob);
}
bool DFS2(int ob, int oj){
    for(int i = 0; i < V[ob].size(); i++)
        if(V[ob][i].first != oj)
            if(DFS2(V[ob][i].first, ob)){
                czys[ob] = true;
                return true;
            }
    if(ob == w2) return true;
    return false;
}
void DFS3(int ob,int oj){
    long long s1 = 0, s2 = 0;
    for(int i = 0; i < V[ob].size(); i++){
        if(V[ob][i].first != oj){
            DFS3(V[ob][i].first, ob);
            if(naj[V[ob][i].first]+V[ob][i].second >= s1){
                s2 = s1;
                s1 = naj[V[ob][i].first]+V[ob][i].second;
            }
            else if(naj[V[ob][i].first]+V[ob][i].second > s2) s2 = naj[V[ob][i].first]+V[ob][i].second;
            dp1[ob] = max(dp1[ob], dp1[V[ob][i].first]);
        }
    }
    dp1[ob] = max(dp1[ob], s1+s2);
    naj[ob] = s1;
}
void DFS4(int ob,int oj){
    long long s1 = 0, s2 = 0;
    for(int i = 0; i < V[ob].size(); i++){
        if(V[ob][i].first != oj){
            DFS4(V[ob][i].first, ob);
            if(naj[V[ob][i].first]+V[ob][i].second >= s1){
                s2 = s1;
                s1 = naj[V[ob][i].first]+V[ob][i].second;
            }
            else if(naj[V[ob][i].first]+V[ob][i].second > s2) s2 = naj[V[ob][i].first]+V[ob][i].second;
            dp2[ob] = max(dp2[ob], dp2[V[ob][i].first]);
        }
    }
    dp2[ob] = max(dp2[ob], s1+s2);
    naj[ob] = s1;
}
void WYNIK(int ob, int oj){
    if(czys[oj] && !czys[ob])
        wyn = max(wyn, odl[w2]+dp1[ob]);
    if(czys[oj] && czys[ob]){
        wyn = max(wyn, dp1[ob]+dp2[oj]);
        for(int i = 0; i < V[ob].size(); i++){
            if(V[ob][i].first != oj)
                WYNIK(V[ob][i].first, ob);
        }
    }
}
int main(){
    int a, b;
    cin >> n;
    for(int i = 1; i < n; i++){
        cin >> a >> b;
        v[a].push_back(b);
        v[b].push_back(a);
    }
    wagi(1, 1);
    DFS(1, 0, 0);
    int w1 = 1;
    long long maxim = 0;
    for(int i = 1; i <= n; i++){
        if(odl[i] > maxim){
            maxim = odl[i];
            w1 = i;
        }
        odl[i] = 0;
    }
    DFS(w1, 0, 0);
    maxim = 0;
    w2 = w1;
    for(int i = 1; i <= n; i++)
        if(odl[i] > maxim){
            maxim = odl[i];
            w2 = i;
        }
    DFS2(w1, 0);
    czys[w2] = true;
    DFS3(w1, 0);
    DFS4(w2, 0);
    int ok = 0;
    for(int i = 0; i < V[w1].size(); i++){
        if(czys[V[w1][i].first])
            ok = V[w1][i].first;
    }
    bool lol = true;
    for(int i = 1; i <= n; i++){
        if(!czys[i])
            lol = false;
    }
    if(lol){
        cout << 0;
        return 0;
    }
    WYNIK(ok, w1);
    cout << suma - wyn;
}
