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

unsigned long long calc_hash(string a){
    using ull = unsigned long long;
    const ull B = 100000007;
    ull al = a.length();
    ull ah = 0;
    for(unsigned int i = 0; i < al; i++) ah = ah * B + a[i];
    return ah;
}

//// 文字列の長い順に並べる
struct Compare{
    bool operator()(const string& lhs, const string& rhs) const{
        return (lhs.length() != rhs.length()) ? lhs.length() > rhs.length() :
                                                //lhs > rhs;
                                                calc_hash(lhs) > calc_hash(rhs);
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

//// 行列を回転させる
vector<string> rot_matrix(const vector<string>& mat){
    vector<string> ret(SIZE, string(SIZE, '.'));
    REP(i,SIZE){
        REP(j,SIZE){
            ret[j][i] = mat[i][j];
        }
    }
    return ret;
}

//// 適当に1文字返す.
char random_char(){
    static mt19937 engine = mt19937(100);;
    static uniform_int_distribution<> rand(0, 7);
    return 'A' + rand(engine);
}

//// inputのうち、answerに含まれている個数を数える
//// O(MN^2)
ll calc_score(vector<string>& input, vector<string>& answer){
    int M = input.size();
    vector<string> rot_answer(SIZE, string(SIZE, '.'));
    REP(i,SIZE){
        REP(j,SIZE){
            rot_answer[j][i] = answer[i][j];
        }
    }

    auto calc = [&](){
        ll ret = 0;
        REP(i, M){
            REP(r,SIZE){
                if(contain(input[i], answer[r]+answer[r]) || contain(input[i], rot_answer[r]+rot_answer[r])){
                    ret++;
                    break;
                }
            }
        }
        return ret;
    };

    ll score = calc();
    return score;
}

//// inputのうち、answerに含まれている個数を数える. (列のみ)
ll calc_score_col(vector<string>& input, vector<string>& answer){
    int M = input.size();
    auto rot = rot_matrix(answer);

    auto calc = [&](){
        ll ret = 0;
        REP(i, M){
            REP(r,SIZE){
                if(contain(input[i], rot[r]+rot[r])){
                    ret++;
                    break;
                }
            }
        }
        return ret;
    };

    ll score = calc();
    return score;
}

//// 行で条件を満たしているinputを取り除く
vector<string> remove_input(const vector<string>& input, const vector<string>& answer){
    vector<string> ret;
    int M = input.size();
    REP(i, M){
        bool remove = false;
        REP(r,SIZE){
            if(contain(input[i], answer[r]+answer[r])){
                remove = true;
                break;
            }
        }

        if(!remove)
            ret.emplace_back(input[i]);
    }

    return ret;
}

int main(){
    ll start = clock();
    ll N, M;
    cin >> N >> M;
    vector<string> answer(N, string(N, '.'));

    vector<string> input(M);
    for(int m = 0; m < M; m++){
        cin >> input[m];
    }

    set<string, Compare> candidate;
    for(auto& s : input)
        candidate.insert(s);

    set<string, Compare> remain;
    //// そのstringがいくつのinputでできているか
    unordered_map<string, int> elms;
    for(auto& s : candidate){
        //// s はrowのどれかに含まれるか？
        bool valid = false;
        for(auto& r : remain){
            if(contain(s, r)){
                valid = true;
                elms[r] += 1;
                break;
            }
        }
        if(valid) continue;

        const int OVERLAP = 3;
        //// s はrowのどれかの一部であるか
        for(auto& r : remain){
            auto check = [&remain, &N, &elms](auto& a, auto& b) {
                int al = a.length(), bl = b.length();
                auto len = overlap(a, b);
                if(len >= OVERLAP){
                    if(al + bl - len > N)
                        return false;
                    auto nstr = a + b.substr(len);
                    remain.insert(nstr);
                    elms[nstr] = elms[a] + elms[b] + 1;
                    ///TODO
                    remain.erase(a);
                    remain.erase(b);
                    elms.erase(a);
                    elms.erase(b);
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
    //cerr << remain.size() << endl;
    //for(auto& r : remain){
    //    cerr << r << endl;
    //}

    //// 含まれる要素数が大きいものから構成
    {
        vector<pair<int, string>> tmp;
        for(auto& [str, cnt] : elms)
            tmp.emplace_back(mp(cnt, str));
        sort(tmp.rbegin(), tmp.rend());
        int row = 0;
        for(auto& p : tmp){
            auto ans = p.second;
            for(int i = ans.length(); i < N; i++)
                ans += random_char();
            answer[row] = ans;
            //del.insert(r);
            row++;
            if(row >= N)
                break;
        }
    }

    input = remove_input(input, answer);
    //cerr << new_input.size() << endl;

    //// TODO: heuristic にanswerの各行の並び順をいじる.
    ll score = calc_score_col(input, answer);
    //const double TIME = 2.8;
    const double TIME = 0.1;
    while(1){
        if((double)(clock() - start) / CLOCKS_PER_SEC > TIME)
            break;
        static mt19937 engine = mt19937(100);;
        static uniform_int_distribution<> rand(0, SIZE-1);

        int row = rand(engine);
        int rot = rand(engine);
        auto tmp = answer[row];
        answer[row] = answer[row].substr(rot) + answer[row].substr(0, rot);
        ll tmp_score = calc_score_col(input, answer);
        if(tmp_score > score){
            score = tmp_score;
        } else {
            answer[row] = tmp;
        }
    }

    print_answer(answer);
    //cerr << calc_score(input, answer) << endl;
    return 0;
}
