#include <iostream>

using namespace std;

int main()
{
    long long n,s,index;
    cin>>n>>s;
    int lista_w[n];
    index=0;
    if (n%2==1)
    {
        n-=1;
        lista_w[index]=0;
        index+=1;

    }
    int liczba;
    liczba=0;
    while (index<n)
    {
        liczba+=1;
        lista_w[index]=liczba;
        index+=1;
        lista_w[index]=liczba*-1;
        index+=1;
    }
    lista_w[index-2]+=s;
    for (int i=0; i<n; i++)
    {
        cout<<lista_w[i]<<endl;
    }
    return 0;
}
