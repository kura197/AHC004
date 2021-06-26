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

//// 文字列の長い順に並べる
struct Compare{
    bool operator()(const string& lhs, const string& rhs) const{
        return (lhs.length() != rhs.length()) ? lhs.length() > rhs.length() :
                                                lhs > rhs;
    }
};

// aはbに含まれているか？
bool contain(string a, string b){
    using ull = unsigned long long;
    const ull B = 100000007;
    int al = a.length();
    int bl = b.length();
    if(al > bl) return false;

    //Bのal乗を計算
    ull t = 1;
    for(int i = 0; i < al; i++) t *= B;

    //aとbの最初のal文字に関するハッシュ値を計算
    ull ah = 0, bh = 0;
    for(int i = 0; i < al; i++) ah = ah * B + a[i];
    for(int i = 0; i < al; i++) bh = bh * B + b[i];

    //bの場所を1ずつ進めながらハッシュ値をチェック
    for(int i = 0; i + al <= bl; i++){
        if(ah == bh) return true;
        if(i + al < bl) bh = bh * B + b[i+al] - b[i] * t;
    }

    return false;
}

// aの末尾とbの先頭を何文字重ねることができるか
int overlap(string a, string b){
    using ull = unsigned long long;
    const ull B = 100000007;
    int al = a.length();
    int bl = b.length();
    int ans = 0;
    ull ah = 0, bh = 0, t = 1;
    for(int i = 1; i <= min(al, bl); i++){
        ah = ah + a[al-i] * t;
        bh = bh * B + b[i-1];
        if(ah == bh) ans = i;
        t *= B;
    }
    return ans;
}

void print_answer(vector<string>& answer){
    REP(i,SIZE){
        cout << answer[i] << endl;
    }
}

void set_naive_answer(vector<string>& answer){
    int N = answer.size();
    REP(i,N){
        REP(j,N){
            static mt19937 engine = mt19937(100);;
            static uniform_int_distribution<> rand(0, 7);
            answer[i][j] = 'A' + rand(engine);
        }
    }
}

//// 取り除いても問題ないものを除去 O(NM^2)
void remove_duplicate(set<string, Compare>& X){
    set<string, Compare> tmp;
    for(auto& s : X){
        int size = s.size();
        bool remove = false;
        for(auto& t : tmp){
            for(int r = 0; r < size; r++){
                string cand = s.substr(r) + s.substr(0, r);
                if(contain(s, t)) {
                    remove = true;
                    break;
                }
            }
        }

        if(!remove)
            tmp.insert(s);
    }

    X = tmp;
}

int main(){
    ll N, M;
    cin >> N >> M;
    vector<string> answer(N, string(N, '.'));

    set<string, Compare> input;
    for(int m = 0; m < M; m++){
        string s;
        cin >> s;
        input.insert(s);
    }

    set<string, Compare> remain;
    for(auto& s : input){
        //// s はrowのどれかに含まれるか？
        bool valid = false;
        for(auto& r : remain){
            if(contain(s, r)){
                valid = true;
                break;
            }
        }
        if(valid) continue;

        //// s はrowのどれかの一部であるか
        for(auto& r : remain){
            const int OVERLAP = 3;
            auto check = [&remain, &N](auto& a, auto& b) {
                int al = a.length(), bl = b.length();
                auto len = overlap(a, b);
                if(len >= OVERLAP){
                    if(al + bl - len > N)
                        return false;
                    //auto test = a + b.substr(len);
                    remain.insert(a + b.substr(len));
                    ///TODO
                    remain.erase(a);
                    remain.erase(b);
                    return true;
                }
                return false;
            };

            if(check(s, r) || check(r, s)){
                valid = true;
                break;
            }
        }
        if(valid) continue;

        remain.insert(s);
    }

    remove_duplicate(remain);
    cerr << remain.size() << endl;
    for(auto& r : remain){
        cerr << r << endl;
    }

    ////TODO
    int row = 0;
    for(auto& r : remain){
        int rl = r.length();
        auto ans = r;
        if(rl > N)
            ans = ans.substr(0, N);
        for(int i = ans.length(); i < N; i++)
            ans += '.';
            //ans += random_char();
        answer[row] = ans;
        row++;
        if(row >= N)
            break;
    }

    print_answer(answer);
    return 0;
}
