#include<bits//stdc++.h>
using namespace std;
float rround(float var)
{
    float value = (int)(var * pow(10, 10) + .5);
    return (float)value / pow(10, 10);
}
int main(){
    int n;
    cin >> n;
    pair<int, float> t[n];
    for(int i = 0; i < n; i++)
        cin >> t[i].second >> t[i].first;
    sort(t, t+n);
    int wyk = t[0].first;
    float sum = t[0].second;
    for(int i = 1; i < n; i++){
        sum /= pow(10, t[i].first-wyk);
        rround(sum);
        cout << setprecision(11) << sum << ' ';
        wyk = t[i].first;
        sum += t[i].second;
    }
    cout << setprecision(11) <<  sum << ' ' << wyk;
}
