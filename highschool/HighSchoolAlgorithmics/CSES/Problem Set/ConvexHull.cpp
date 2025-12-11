
#include<bits//stdc++.h>
using namespace std;

struct point{
    long long x;
    long long y;
};

bool operator==(const point& a, const point& b)
{
    return (a.x == b.x && a.y == b.y);
}

point start; // punkt o najmniejszej współżędnej y

// sortowanie kątowe
bool katsort(point a, point b){
    if(a == start) return true;
    if(b == start) return false;
    if((a.y-start.y)*(b.x-start.x) == (a.x-start.x)*(b.y-start.y))
        return (pow((a.x-start.x), 2)+pow((a.y-start.y), 2) < pow((start.x-b.x), 2)+pow((start.y-b.y), 2));
    return ((a.y-start.y)*(b.x-start.x) < (a.x-start.x)*(b.y-start.y));
}

// iloczyn skalarny
long long dot(point a, point b, point c){
    return (c.x-a.x)*(b.y-a.y)-(c.y-a.y)*(b.x-a.x);
}
vector<point> fin;
//wyliczanie otoczki wypukłej wielokąta v
void otoczka(vector<point> &v){
    sort(v.begin(), v.end(), katsort);
    int i = v.size()-1;
    fin.push_back(v[i]);
    while((v[i].y-start.y)*(v[i-1].x-start.x) == (v[i].x-start.x)*(v[i-1].y-start.y)){
        i--;
        fin.push_back(v[i]);
    }
    stack<point> s;
    s.push(v[0]);
    s.push(v[1]);
    point ob1 = v[0], ob2 = v[1];
    for(int i = 2; i < v.size(); i++){
        if(dot(ob1, ob2, v[i]) <= 0){
            s.push(v[i]);
            swap(ob1, ob2);
            ob2 = v[i];
            continue;
        }
        s.pop();
        s.pop();
        while(dot(ob1, ob2, v[i]) > 0){
            swap(ob1, ob2);
            ob1 = s.top();
            s.pop();
        }
        s.push(ob1);
        s.push(ob2);
        s.push(v[i]);
        swap(ob1, ob2);
        ob2 = v[i];
    }
    v.clear();
    while(!s.empty()){
        v.push_back(s.top());
        s.pop();
    }
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    point p;
    vector<point> v;
    cin >> start.x >> start.y;
    v.push_back(start);
    for(int i = 1; i < n; i++){
        cin >> p.x >> p.y;
        v.push_back(p);
        if(start.y > p.y) start = p;
    }
    otoczka(v);
    cout << v.size()+fin.size()-1 << '\n';
    for(int i = 0; i < v.size(); i++)
        cout << v[i].x << ' ' << v[i].y << '\n';
    for(int i = 1; i < fin.size(); i++)
        cout << fin[i].x << ' ' << fin[i].y << '\n';
}
