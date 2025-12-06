#include<bits//stdc++.h>
using namespace std;
char t[1000001];
int n, M[100001][10][10];
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
int W(int i, int stan1, int stan2){
    if(M[i][stan1][stan2] != -1)
            return M[i][stan1][stan2];
    else{
        if(i == n) M[i][stan1][stan2] = max(new_state(stan1, t[i]).second, new_state(stan2, t[i]).second);
        else M[i][stan1][stan2] = max(W(i+1, new_state(stan1, t[i]).first, stan2) + new_state(stan1, t[i]).second,
                   W(i+1, stan1, new_state(stan2, t[i]).first) + new_state(stan2, t[i]).second);
    }
    return M[i][stan1][stan2];
}
int main(){
    cin >> n;
    for(int i = 1; i <= n; i++)
        cin >> t[i];
    for(int i = 0; i <= n; i++){
        for(int y = 0; y < 10; y++){
            for(int x = 0; x < 10; x++){
                M[i][y][x] = -1;
            }
        }
    }
    /* STATES
    0 = null |
    1 = MM    |   4 = MB    |   7 = MF
    2 = BM    |   5 = BB    |   8 = BF
    3 = FM    |   6 = FB    |   9 = FF
    */
    cout << W(1, 0, 0);

}
