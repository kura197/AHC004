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

void print_answer_debug(vector<string>& answer){
    REP(i,SIZE){
        cerr << answer[i] << endl;
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
char random_char(mt19937& engine){
    //static mt19937 engine = mt19937(100);;
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

//// あるqueryに包含されているqueryを除去する
set<string, Compare> remove_contain(set<string, Compare>& vec, auto& elms){
    set<string, Compare> ret;
    for(auto& s : vec){
        //// s はrowのどれかに含まれるか？
        bool valid = false;
        for(auto& r : ret){
            if(contain(s, r)){
                valid = true;
                elms[r] += elms[s];
                elms[s] = 0;
                elms.erase(s);
                break;
            }
        }
        if(valid) continue;
        ret.insert(s);
    }
    return ret;
}

//// threshold 以上の重なりのある文字列を結合する
set<string, Compare> append_query(set<string, Compare>& vec, auto& elms, int threshold){
    set<string, Compare> ret;
    for(auto& s : vec){
        string rem;
        string nstr;
        int max_len = -1;
        for(auto& r : ret){
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
            } 
            else {
                if(sl + rl - r_len <= N && max_len < r_len){
                    nstr = r + s.substr(r_len);
                    max_len = r_len;
                    rem = r;
                }
            }
        }

        if(max_len >= threshold){
            ret.insert(nstr);
            elms[nstr] = elms[s] + elms[rem];
            ret.erase(s);
            ret.erase(rem);
            elms.erase(s);
            elms.erase(rem);
            continue;
        }

        ret.insert(s);
    }
    return ret;
}

//// 長さがN以下となるように、queryを結合する
set<string, Compare> gather_query(set<string, Compare>& vec, auto& elms){
    set<string, Compare> ret;
    for(auto& s : vec){
        bool valid = false;
        string del;
        for(auto& r : ret){
            int sl = s.length();
            int rl = r.length();
            if(sl + rl <= N){
                auto nstr = s + r;
                ret.insert(nstr);
                elms[nstr] = elms[s] + elms[r];
                elms.erase(s);
                elms.erase(r);
                del = r;
                valid = true;
                break;
            }
        }

        if(valid)
            ret.erase(del);
        else
            ret.insert(s);
    }
    return ret;
}

string rotate_str(string a, int rot){
    return a.substr(rot) + a.substr(0, rot);
}

void print_elms_sum(auto& elms){
    ll sum = 0;
    for(auto& p : elms){
        sum += p.second;
    }
    cerr << "SUM : " << sum << endl;
}

vector<string> make_answer_by_high_elms(auto& elms, auto& remain, auto& input, int start, double time, mt19937& engine){
    vector<string> answer(N, string(N, '.'));
    //// 含まれる要素数が大きいものから構成
    vector<pair<int, string>> tmp;
    for(auto& [str, cnt] : elms)
        tmp.emplace_back(mp(cnt, str));
    sort(tmp.rbegin(), tmp.rend());
    int row = 0;
    int idx = 0;
    for(; idx < (int)tmp.size(); idx++){
        if(row >= N)
            break;
        auto ans = tmp[idx].second;
        cerr << ans << " : "  << elms[ans] << endl;
        for(int i = ans.length(); i < N; i++)
            ans += random_char(engine);
        answer[row] = ans;
        row++;
        remain.erase(tmp[idx].second);
    }

    //print_answer_debug(answer);

    //// answerの各行を回転し、tmpと列ごとに一致させていく
    //for(; idx < (int)tmp.size(); idx++){
    //}

    //// 行で条件を満たすinputを取り除く
    input = remove_input(input, answer);

    vector<int> indices(N);
    REP(i,N) indices[i] = i;

    ll score = calc_score_col(input, answer);
    //// 時間まで、行を適当にシャッフル --> idx番目が列に含まれるように回転 を繰り返す。
    int t = idx;
    while(1){
        if((double)(clock() - start) / CLOCKS_PER_SEC > time)
            break;

        string target = tmp[t].second;

        shuffle(indices.begin(), indices.end(), engine);
        vector<string> cand(N);
        REP(i,N){
            cand[i] = answer[indices[i]];
        }

        //static uniform_int_distribution<> rand(0, SIZE-1);
        //target = rotate_str(target, rand(engine));
        bool valid = true;
        for(int i = 0; i < (int)target.length(); i++){
            char c = target[i];
            bool found = false;
            for(int j = 0; j < N; j++){
                if(cand[i][j] == c){
                    found = true;
                    cand[i] = rotate_str(cand[i], j);
                    break;
                }
            }

            if(!found){
                valid = false;
                break;
            }
        }

        if(!valid) continue;

        ll tmp_score = calc_score_col(input, cand);

        if(tmp_score > score){
            score = tmp_score;
            answer = cand;
        }     

        t++;
        if(t == (int)tmp.size())
            t = idx;
    }

    //cerr << endl;
    //print_answer_debug(answer);
    return answer;
}

void put_by_greedy(string& s, auto& mat, mt19937& engine){
    int sl = s.length();
    int row = -1, col = -1;
    int max_score = -1;
    int bestDir = -1;

    int match = 0;
    REP(i,N){
        REP(j,N){
            REP(dir,2){
                int tmp_score = 0;
                for(int k = 0; k < sl; k++){
                    int y = (dir == 0) ? (i + k) % N : i;
                    int x = (dir == 1) ? (j + k) % N : j;
                    if(s[k] == mat[y][x])
                        tmp_score += 2;
                    else if(mat[y][x] == '.')
                        tmp_score += 1;
                }
                if(tmp_score > max_score){
                    max_score = tmp_score;
                    row = i; 
                    col = j;
                    bestDir = dir;
                }
                else if(tmp_score == max_score){
                    static uniform_int_distribution<> rand(0, 65535);
                    match++;
                    if(rand(engine) % match == 0){
                        row = i; 
                        col = j;
                        bestDir = dir;
                    }
                }
            }
        }
    }

    if(max_score != -1){
        for(int k = 0; k < sl; k++){
            int y = (bestDir == 0) ? (row + k) % N : row;
            int x = (bestDir == 1) ? (col + k) % N : col;
            if(mat[y][x] == '.')
                mat[y][x] = s[k];
        }
    }
}

void put_by_greedy_iwashi(string& s, auto& mat, mt19937& engine){
    int sl = s.length();
    int row = -1, col = -1;
    int bestDir = -1;
    int bestDiff = N;

    int match = 0;
    REP(i,N){
        REP(j,N){
            REP(dir,2){
                //int tmp_score = 0;
                bool valid = true;
                int diff = 0;
                for(int k = 0; k < sl; k++){
                    int y = (dir == 0) ? (i + k) % N : i;
                    int x = (dir == 1) ? (j + k) % N : j;
                    if(s[k] != mat[y][x]){
                        if(mat[y][x] == '.')
                            diff++;
                        else{
                            valid = false;
                            break;
                        }
                    }
                }

                if(valid){
                    if(diff < bestDiff){
                        bestDiff = diff;
                        row = i; 
                        col = j;
                        bestDir = dir;
                        match = 1;
                    }
                    else if(diff == bestDiff){
                        static uniform_int_distribution<> rand(0, 65535);
                        match++;
                        if(rand(engine) % match == 0){
                            row = i; 
                            col = j;
                            bestDir = dir;
                        }
                    }
                }
            }
        }
    }

    if(match > 0){
        for(int k = 0; k < sl; k++){
            int y = (bestDir == 0) ? (row + k) % N : row;
            int x = (bestDir == 1) ? (col + k) % N : col;
            if(mat[y][x] == '.')
                mat[y][x] = s[k];
        }
    }
}

vector<string> make_answer_by_greedy(auto& elms, auto& remain, auto& input, int start, double time, mt19937& engine){
    //vector<pair<int, string>> tmp;
    //for(auto& [str, cnt] : elms)
    //    tmp.emplace_back(mp(cnt, str));
    //sort(tmp.rbegin(), tmp.rend());

    //vector<string> strs;
    //for(auto& p : tmp)
    //    strs.emplace_back(p.second);
    
    vector<string> strs;
    for(auto& p : remain)
        strs.emplace_back(p);

    sort(strs.begin(), strs.end(), [](const string& lhs, const string& rhs){
                return lhs.size() > rhs.size();
            });

    ll score = -1;
    vector<string> answer(N, string(N, '.'));
    while(1){
        vector<string> cand(N, string(N, '.'));
        for(auto& s : strs){
            put_by_greedy_iwashi(s, cand, engine);
        }

        //ll tmp_score = calc_score_col(input, cand);
        ll tmp_score = calc_score(input, cand);

        if(tmp_score > score){
            score = tmp_score;
            answer = cand;
        }     

        shuffle(strs.begin(), strs.end(), engine);
        sort(strs.begin(), strs.end(), [](const string& lhs, const string& rhs){
                return lhs.size() > rhs.size();
                });

        if((double)(clock() - start) / CLOCKS_PER_SEC > time)
            break;
    }

    return answer;
}

void append_inputs(vector<string>& input, const int threshold){
rerun:
    int size = input.size();
    REP(i,size){
        REP(j,size){
            if(i == j) continue;
            int il = input[i].length();
            int jl = input[j].length();
            int len = overlap(input[i], input[j]);
            if(len >= threshold && il + jl - len <= N){
                input[i] += input[j].substr(len);
                input.erase(input.begin() + j);
                goto rerun;
            }
        }
    }
}

void merge_inputs(vector<string>& input){
rerun:
    int size = input.size();
    REP(i,size){
        REP(j,size){
            if(i == j) continue;
            int il = input[i].length();
            int jl = input[j].length();
            if(il + jl <= N){
                input[i] += input[j];
                input.erase(input.begin() + j);
                goto rerun;
            }
        }
    }
}
vector<string> solve_iwashi(vector<string> input, double time, int threshold, mt19937& engine){
    ll start = clock();
    vector<string> answer(N, string(N, '.'));

    append_inputs(input, threshold);
    //merge_inputs(input);

    for(auto& s : input){
        cerr << s << endl;
    }

    answer = make_answer_by_greedy(input, input, input, start, time, engine);

    return answer;
}

vector<string> solve(vector<string> input, double time, int threshold, mt19937& engine){
    ll start = clock();
    vector<string> answer(N, string(N, '.'));

    //// そのstringがいくつのinputでできているか
    unordered_map<string, int> elms;

    for(auto& s : input)
        elms[s] += 1;
    
    set<string, Compare> remain;
    for(auto& s : input)
        remain.insert(s);

    //for(int thres = threshold; thres >= 1; thres--){
    while(1){
        const int thres = threshold;
        int size = remain.size();
        remain = remove_contain(remain, elms);
        remain = append_query(remain, elms, thres);
        if(size == (int)remain.size())
            break;
    }
    remain = gather_query(remain, elms);

    //remove_duplicate(remain);
    
    cerr << remain.size() << endl;
    for(auto& r : remain){
        cerr << r << " " << elms[r] << endl;
    }

    //answer = make_answer_by_high_elms(elms, remain, input, start, time, engine);
    answer = make_answer_by_greedy(elms, remain, input, start, time, engine);
    return answer;
}

int main(){
    cin >> N >> M;

    vector<string> input(M);
    for(int m = 0; m < M; m++){
        cin >> input[m];
    }

    const double TIME = 2.9;
    //const double TIME = 0.4;

    ll score = -1;
    vector<string> answer;
    //// duplicated
    vector<int> thresholds{5};
    vector<int> seeds{1};
    //vector<int> thresholds{2, 3, 4, 5};
    //vector<int> seeds{1, 100, 10000};
    const int iteration = thresholds.size() * seeds.size();
    for(auto& thres : thresholds){
        for(auto& seed : seeds){
            mt19937 engine = mt19937(seed);
            //auto cand = solve(input, TIME/iteration, thres, engine);
            auto cand = solve_iwashi(input, TIME/iteration, thres, engine);
            auto tmp_score = calc_score(input, cand);
            if(tmp_score > score){
                score = tmp_score;
                answer = cand;
            }
        }
    }

    print_answer(answer);
    //cerr << calc_score(input, answer) << endl;
    return 0;
}
