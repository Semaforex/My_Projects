#include <iostream>
using namespace std;
int main(){
    int m,v,w,u;
    cin>>m;
    if(m%2==1){return 0;}
    else{
        v=0;
        for(int i=0; i<m/2; i++){
            cin>>w>>u;
            if(u%2!=w%2){
                int z=w;
                w=u;
                u=z;
            }
            w=w/10;
            u=u*u*u;
            if(u==w){
                v=v+1;
            }
        }
        cout<<v;
    }
}
