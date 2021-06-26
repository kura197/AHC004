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

ll N, M;

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

/// xor128
unsigned int randxor()
{
    static unsigned int x=123456789, y=362436069, z=521288629, w=88675123;
    unsigned int t;
    t = (x^(x<<11));
    x = y;
    y = z;
    z = w; 
    return( w=(w^(w>>19))^(t^(t>>8)) );
}

vector<string> solve(vector<string> input, double time, int threshold){
    ll start = clock();
    vector<string> answer(N, string(N, '.'));
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

        {
            const int OVERLAP = threshold;
            string rem;
            string nstr;
            int max_len = -1;
            for(auto& r : remain){
                auto s_len = overlap(s, r);
                auto r_len = overlap(r, s);
                int sl = s.length();
                int rl = r.length();
                if(s_len > r_len){
                    if(sl + rl - s_len <= N && max_len < s_len){
                        nstr = s + r.substr(s_len);
                        max_len = s_len;
                        rem = r;
                    }
                } else {
                    if(sl + rl - r_len <= N && max_len < r_len){
                        nstr = r + s.substr(r_len);
                        max_len = r_len;
                        rem = r;
                    }
                }
            }

            if(max_len >= OVERLAP){
                remain.insert(nstr);
                elms[nstr] = elms[s] + elms[rem] + 1;
                remain.erase(s);
                remain.erase(rem);
                elms.erase(s);
                elms.erase(rem);
                valid = true;
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
            //cerr << ans << endl;
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

    //const int iteration = 500;
    //const double startTemp = 100;
    //const double endTemp = 1;
    //const int R = 10000;
    //const int T = iteration;
    //int t = 0;
    while(1){
        if((double)(clock() - start) / CLOCKS_PER_SEC > time)
            break;
        static mt19937 engine = mt19937(100);;
        static uniform_int_distribution<> rand(0, SIZE-1);

        //const int NUM = (t < 25) ? 4 : (t < 50) ? 3 : (t < 75) ? 2 : 1;
        const int NUM = 1;
        vector<int> row(NUM);
        vector<int> rot(NUM);
        vector<string> org(NUM);
        for(int i = 0; i < NUM; i++){
            row[i] = rand(engine);
            rot[i] = rand(engine);
            org[i] = answer[row[i]];
            answer[row[i]] = answer[row[i]].substr(rot[i]) + answer[row[i]].substr(0, rot[i]);
        }

        ll tmp_score = calc_score_col(input, answer);
        //double temp = startTemp + (endTemp - startTemp) * t / T;
        //double probability = exp((tmp_score - score) / temp);
        //bool force_next = probability > (double)(randxor() % R) / R;
        //cerr << t << " " << probability << " " << tmp_score << " " << score << endl;
        //if(tmp_score > score || force_next){
        if(tmp_score > score){
            score = tmp_score;
        } else {
            for(int i = NUM-1; i >= 0; i--)
                answer[row[i]] = org[i];
        }

        //t++;
    }

    return answer;
}

int main(){
    cin >> N >> M;

    vector<string> input(M);
    for(int m = 0; m < M; m++){
        cin >> input[m];
    }

    //const double TIME = 2.8;
    const double TIME = 1.0;

    ll score = 0;
    vector<string> answer;
    vector<int> thresholds{2, 3, 4};
    for(auto& thres : thresholds){
        auto cand = solve(input, TIME/thresholds.size(), thres);
        auto tmp_score = calc_score(input, cand);
        if(tmp_score > score){
            score = tmp_score;
            answer = cand;
        }
    }

    print_answer(answer);
    //cerr << calc_score(input, answer) << endl;
    return 0;
}
