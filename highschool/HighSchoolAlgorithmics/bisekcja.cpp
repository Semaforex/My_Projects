#include <bits//stdc++.h>
using namespace std;

//wspó³czynniki wielomianu
vector<double> wiel;

//funckja
double f(double x){
    double w = 0;
    for(int i = 0; i < wiel.size(); i++)
        w += wiel[i]*pow(x, i);
    return w;
}

//root
double e, a, b;
double root(){
    if(f(a)*f(b) >= 0) return -1;
    if(f(a) > f(b))
        while(b-a > e){
            double mid = (a+b)/2;
            if(f(mid) > 0) a = mid;
            else b = mid;
        }
    else
        while(b-a > e){
            double mid = (a+b)/2;
            if(f(mid) > 0) b = mid;
            else a = mid;
        }
    return (a+b)/2;
}
int main(){
    int n, k;
    cout << "number of examples\n";
    cin >> n;
    cout << "precision\n";
    cin >> e;
    for(int i = 0; i < n; i++){
        wiel.clear();
        double x;
        cout << "polynomial degree\n";
        cin >> k;
        cout << "polynomial coefficients (from smallest to biggest)\n";
        for(int y = 0; y < k; y++){
            cin >> x;
            wiel.push_back(x);
        }
        cout << "interval boundaries\n";
        cin >> a >> b;
        cout << "root is in x=" << root();
    }
}
