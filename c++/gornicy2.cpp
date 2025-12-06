#include<bits//stdc++.h>
using namespace std;
char t[100001];
int n, pop[10][10], ob[10][10], mx = 0;
pair<int, int> new_state(int state, char ob){
    if(ob == 'M'){
        if(state == 0 || state == 1) return make_pair(1, 1);
        if(state == 2 || state == 3) return make_pair(1, 2);
        if(state == 4 || state == 5) return make_pair(2, 2);
        if(state == 7 || state == 9) return make_pair(3, 2);
        if(state == 8) return make_pair(3, 3);
        if(state == 6) return make_pair(2, 3);
    }
    if(ob == 'B'){
        if(state == 0 || state == 5) return make_pair(5, 1);
        if(state == 1 || state == 2) return make_pair(4, 2);
        if(state == 4 || state == 6) return make_pair(5, 2);
        if(state == 8 || state == 9) return make_pair(6, 2);
        if(state == 3) return make_pair(4, 3);
        if(state == 7) return make_pair(6, 3);
    }
    if(ob == 'F'){
        if(state == 0 || state == 9) return make_pair(9, 1);
        if(state == 1 || state == 3) return make_pair(7, 2);
        if(state == 5 || state == 6) return make_pair(8, 2);
        if(state == 7 || state == 8) return make_pair(9, 2);
        if(state == 2) return make_pair(7, 3);
        if(state == 4) return make_pair(8, 3);
    }

}
int main(){
    cin >> n;
    for(int i = 1; i <= n; i++)
        cin >> t[i];
    for(int y = 0; y < 10; y++){
        for(int x = 0; x < 10; x++){
            pop[y][x] = -1;
            ob[y][x] = -1;
        }
    }
    pop[0][0] = 0;
    for(int i = 1; i <= n; i++){
        for(int k = 0; k < 10; k++){
            for(int l = 0; l < 10; l++){
                if(pop[k][l] != -1){
                    int stan1 = new_state(k, t[i]).first;
                    int stan2 = new_state(l, t[i]).first;
                    int w1 = new_state(k, t[i]).first;
                    int w2 = new_state(l, t[i]).first;
                    if(w1 > ob[stan1][l]) ob[stan1][l] = w1;
                    if(w2 > ob[k][stan2]) ob[k][stan2] = w2;
                    cout << stan1 << ' ' << l << '\n' << k << ' ' << stan2 << '\n';
                }
            }
        }
        for(int k = 0; k < 10; k++)
            for(int l = 0; l < 10; l++)
                pop[k][l] = ob[k][l];
        for(int k = 0; k < 10; k++)
            for(int l = 0; l < 10; l++)
                ob[k][l] = -1;
    }
    /* STATES
    0 = null |
    1 = MM    |   4 = MB    |   7 = MF
    2 = BM    |   5 = BB    |   8 = BF
    3 = FM    |   6 = FB    |   9 = FF
    */
    for(int k = 0; k < 10; k++)
            for(int l = 0; l < 10; l++)
                    mx = max(mx, pop[k][l]);

    cout << mx;

}
