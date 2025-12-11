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
bool katsort(pair<point, int> ta, pair<point, int> tb){
    point a = ta.first;
    point b = tb.first;
    int posa = ta.second, posb = tb.second;
    if(a == start) return true;
    if(b == start) return false;
    if((a.y-start.y)*(b.x-start.x) == (a.x-start.x)*(b.y-start.y))
        return posa < posb;
    return ((a.y-start.y)*(b.x-start.x) < (a.x-start.x)*(b.y-start.y));
}

// iloczyn skalarny
long long dot(point a, point b, point c){
    return (c.x-a.x)*(b.y-a.y)-(c.y-a.y)*(b.x-a.x);
}

// pole trójkąta abc
long long triangle(point a, point b, point c){
    return abs((b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y));
}

// zmiana kolejności punktów wielokąta by pierwszy był start i żeby poruszać się po wielokącie zgodnie z ruchem wskazówek zegara
void modify(vector<point> &v, int i){
    int s = v.size();
    if(i == 0){
        if(dot(v[s-1], v[0], v[1]) > 0){
            reverse(v.begin(), v.end());
            i = s-1;
        }
    }
    else{
        if(i == s-1){
            if(dot(v[s-2], v[s-1], v[0]) > 0){
                reverse(v.begin(), v.end());
                i = 0;
            }
        }
    }
    if(i > 0 && i < s-1){
        if(dot(v[i-1], v[i], v[i+1]) > 0){
            reverse(v.begin(), v.end());
            i = s-i-1;
        }
    }


    vector<point> temp(v.begin(), v.begin()+i);
    v.erase(v.begin(), v.begin()+i);
    v.insert(v.end(), temp.begin(), temp.end());
}

//wyliczanie otoczki wypukłej wielokąta v
void otoczka(vector <point> &v){
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

//rekurencyjna funckja obliczająca pole powierzchni v
long long Area(vector<point> &v){
    if(v.size() == 3) return triangle(v[0], v[1], v[2]);
    point p = v[0];
    for(int i = 1; i < v.size(); i++){
        if(v[i].y < p.y)
            p = v[i];
    }
    start = p;
    int i = 0;
    while(!(v[i] == start)) i++;
    modify(v, i);
    vector<point> u = v;
    vector<pair<point, int>> tem;
    for(int i = 0; i < v.size(); i++) tem.push_back({v[i], i});
    sort(tem.begin(), tem.end(), katsort);
    v.clear();
    for(int i = 0; i < tem.size(); i++) v.push_back(tem[i].first);
    otoczka(v);
    reverse(v.begin(), v.end());
    long long A = 0;
    for(int i = 1; i < v.size()-1; i++)
        A += triangle(v[0], v[i], v[i+1]);
    i = 1;
    int y = 1;
    vector<point> temp;
    while(i < u.size()){
        while(v[y] == u[i]){
            if(i == u.size()-1) return A;
            if(y == v.size()-1){
                temp.push_back(v[v.size()-1]);
                while(i < u.size()-1){
                    i++;
                    temp.push_back(u[i]);
                }
                temp.push_back(v[0]);
                return A-Area(temp);
            }
            i++;
            y++;
        }
        temp.push_back(u[i-1]);
        while(!(v[y] == u[i])){
            temp.push_back(u[i]);
            i++;
        }
        temp.push_back(u[i]);
        A -= Area(temp);
        temp.clear();
    }
    return A;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    point a;
    vector<point> v;
    for(int i = 0; i < n; i++){
        cin >> a.x >> a.y;
        v.push_back(a);
    }
    vector<point> u = v;
    cout << Area(v);
}
