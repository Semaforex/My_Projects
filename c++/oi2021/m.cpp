#include<iostream>
#include<queue>
#include<vector>
#include<algorithm>
using namespace std;

struct film{
    int data;
    int dni;
    int pos;
};
struct comp
{
    bool operator()(film a, film b)
    {
        return b.dni > a.dni;
    }
};
bool comp2(film a, film b)
{
    return a.data < b.data;
}

pair<int, int> t[500001];
int n;
priority_queue<film, vector<film>, comp> q;
long long dost;
vector<film> odp;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> n;
    for(int i = 1; i <= n; i++)
        cin >> t[i].second >> t[i].first;
    sort(t+1, t+n+1);
    for(int i = 1; i <= n; i++){
        dost += t[i].first-t[i-1].first;
        if(dost >= t[i].second){
            dost -= t[i].second;
            film ob;
            ob.data = t[i].first;
            ob.dni = t[i].second;
            ob.pos = i;
            q.push(ob);
            continue;
        }
        if(!q.empty() && t[i].second < q.top().dni){
            dost += q.top().dni;
            dost -= t[i].second;
            q.pop();
            film ob;
            ob.data = t[i].first;
            ob.dni = t[i].second;
            ob.pos = i;
            q.push(ob);
        }
    }
    cout << q.size() << '\n';
    while(!q.empty()){
        odp.push_back(q.top());
        q.pop();
    }
    /*sort(odp.begin(), odp.end(), comp2);
    long long x = 1;
    for(int i = 0; i < odp.size(); i++){
        cout << odp[i].pos << ' ' << x << '\n';
        x += odp[i].dni;
    }*/
}
