#include <bits/stdc++.h>

using namespace std;
vector<pair<long long, long long>> G[5001];
vector<pair<long long, long long>> G2[5001];
long long wartosci[5001];
long long wartosci2[5001];
long long ceny_metali[5001];
long long liczba_wierzcholkow, liczba_krawedzi;
long long inf = 2e15;

void dijkstra(long long v)
{
    wartosci[v] = 0;
    set<pair<long long, long long>> s;

    s.insert({0, v});

    while(!s.empty())
    {
        pair <long long, long long> a = *s.begin();
        s.erase(a);

        for(auto i : G[a.second])
        {
            if(wartosci[i.second] > wartosci[a.second] + i.first)
            {
                s.erase({wartosci[i.second], i.second});

                wartosci[i.second] = wartosci[a.second] + i.first;

                s.insert({wartosci[i.second], i.second});
            }
        }
    }
}

long long dijkstra2(long long v)
{
    wartosci2[v] = 0;
    set<pair<long long, long long>> s;

    s.insert({0, v});

    while(!s.empty())
    {
        pair <long long, long lo
