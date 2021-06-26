#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
#define REP(i, n) for(int i=0; i<n; i++)
#define REPi(i, a, b) for(int i=int(a); i<int(b); i++)
#define MEMS(a,b) memset(a,b,sizeof(a))
#define mp make_pair
#define MOD(a, m) ((a % m + m) % m)
template<class T>bool chmax(T &a, const T &b) { if (a<b) { a=b; return 1; } return 0; }
template<class T>bool chmin(T &a, const T &b) { if (b<a) { a=b; return 1; } return 0; }
const ll MOD = 1e9+7;

/// matrix size
const ll SIZE = 20;

void print_answer(vector<string>& answer){
    REP(i,SIZE){
        cout << answer[i] << endl;
    }
}

int main(){
    ll N, M;
    cin >> N >> M;
    vector<string> answer(N, string(N, '.'));

    for(int m = 0; m < M; m++){
        string S;
        cin >> S;
    }

    print_answer(answer);
    return 0;
}
