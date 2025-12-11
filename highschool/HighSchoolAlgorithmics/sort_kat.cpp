#include <bits//stdc++.h>
using namespace std;
struct point{
    int x;
    int y;
};
point p(int x, int y){
    point a;
    a.x = x;
    a.y = y;
    return a;
}
bool comp(point a, point b){
    long long p = a.x*b.y, q = a.y*b.x;
    return p > q;
}
int main(){
    int n, x, y;
    cin >> n;
    vector<point> I, II, III, IV;
    for(int i = 0; i < n; i++){
        cin >> x >> y;
        if(x > 0 && y >= 0) I.push_back(p(x, y));
        if(x <= 0 && y > 0) II.push_back(p(-x, y));
        if(x < 0 && y <= 0) III.push_back(p(-x, -y));
        if(x >= 0 && y < 0) IV.push_back(p(x, -y));
    }
    sort(I.begin(), I.end(), comp);
    sort(II.begin(), II.end(), comp);
    sort(III.begin(), III.end(), comp);
    sort(IV.begin(), IV.end(), comp);
    for(int i = 0; i < II.size(); i++)
        I.push_back(p(-II[i].x, II[i].y));
    for(int i = 0; i < III.size(); i++)
        I.push_back(p(-III[i].x, -III[i].y));
    for(int i = 0; i < IV.size(); i++)
        I.push_back(p(IV[i].x, -IV[i].y));
    for(int i = 0; i < I.size(); i++)
        cout << '(' << I[i].x << ", " << I[i].y << ") ";
}
