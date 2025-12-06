#include <bits//stdc++.h>

using namespace std;

int pierw(long long* dane, int dl, int L, int P, long long a) {
	if (P >= L) {
		int M = (L + P) / 2;
		if ((M == 0 || a > dane[M - 1]) && dane[M] == a) {
			return M;
		} else if (a > dane[M]) {
			return pierw(dane, dl, M + 1, P, a);
		} else {
			return pierw(dane, dl, L, M - 1, a);
		}
	}
	return -1;
}

int kon(long long* dane, int dl, int L, int P, long long a) {
	if (P >= L) {
		int M = (L + P) / 2;
		if ((M == dl - 1 || a < dane[M + 1]) && dane[M] == a) {
			return M;
		} else if (a < dane[M]) {
			return kon(dane, dl, L, M - 1, a);
		} else {
			return kon(dane, dl, M + 1, P, a);
		}
	}
	return -1;
}

int ile(long long* dane, int dl, long long a) {
	int wystL = 0, wystP = 0;
	wystL = pierw(dane, dl, 0, dl - 1, a);
	wystP = kon(dane, dl, 0, dl - 1, a);
	if (wystL == -1 || wystP == -1)
		return 0;
	return wystP - wystL + 1;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
	int n;
	cin >> n;
	long long t[n];
	for(int i = 0; i < n; i++) cin >> t[i];
	sort(t, t+n);
	int q;
	cin >> q;
	for(int i = 0; i < q; i++){
        long long x;
        cin >> x;
        cout << ile(t, n, x) << '\n';
	}
	return 0;
}
