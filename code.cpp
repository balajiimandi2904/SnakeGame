#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <chrono>
#include <climits>
#include <string>
#include <sstream>
#include <set>
#include <queue>

using namespace std;

int N;
vector<vector<int> > adj; // Note: space between > > for older compilers
map<vector<int>, int> memo_game;

struct GameResult {
    int length;
    bool reached;
};

// Forward declarations
int solve_adversary(int u, long long mask, vector<int> path);
GameResult solve_snake(int u, int target, long long mask, vector<int> path, set<vector<int> >& history, int depth_limit);

// Replaced C++17 structured bindings with C++11 compatible code
int solve_adversary(int u, long long mask, vector<int> path) {
    if (memo_game.count(path)) return memo_game[path];

    int best_min_len = INT_MAX;
    bool placed = false;

    for (int apple = 0; apple < N; apple++) {
        if (!((mask >> apple) & 1)) {
            placed = true;
            set<vector<int> > history;
            history.insert(path);
            
            // Limit search depth to N to prevent infinite shuffling loops
            GameResult res = solve_snake(u, apple, mask, path, history, N);
            if (res.length < best_min_len) {
                best_min_len = res.length;
            }
        }
    }
    return memo_game[path] = (placed ? best_min_len : (int)path.size());
}

GameResult solve_snake(int u, int target, long long mask, vector<int> path, set<vector<int> >& history, int depth_limit) {
    if (u == target) return {solve_adversary(u, mask, path), true};
    if (depth_limit <= 0) return {(int)path.size(), false};

    int best_max = (int)path.size();
    bool can_reach = false;

    for (size_t i = 0; i < adj[u].size(); i++) {
        int v = adj[u][i];
        bool is_tail = (!path.empty() && v == path.front());
        if (!((mask >> v) & 1) || is_tail) {
            vector<int> next_p = path;
            long long next_m = mask;
            
            if (v == target) {
                next_p.push_back(v);
                next_m |= (1LL << v);
            } else {
                next_m &= ~(1LL << next_p.front());
                next_p.erase(next_p.begin());
                next_p.push_back(v);
                next_m |= (1LL << v);
            }

            if (history.find(next_p) == history.end()) {
                history.insert(next_p);
                GameResult res = solve_snake(v, target, next_m, next_p, history, depth_limit - 1);
                if (res.reached) {
                    can_reach = true;
                    if (res.length > best_max) best_max = res.length;
                }
                history.erase(next_p);
            }
        }
    }
    return {best_max, can_reach};
}

int main() {
    string line;
    while (getline(cin, line)) {
        if (line.empty() || line.find("Graph") == string::npos) continue;
        
        string graphHeader = line;
        size_t orderPos = line.find("order");
        stringstream ss_meta(line.substr(orderPos + 6));
        ss_meta >> N;

        adj.assign(N, vector<int>());
        for (int i = 0; i < N; i++) {
            getline(cin, line);
            replace(line.begin(), line.end(), ':', ' ');
            replace(line.begin(), line.end(), ';', ' ');
            stringstream ss_edge(line);
            int u, v;
            ss_edge >> u;
            while (ss_edge >> v) adj[u].push_back(v);
        }

        //checking for degree one vertex

        // --- Trivial Case Optimization ---
        bool has_degree_one = false;
        for (int i = 0; i < N; i++) {
            if (adj[i].size() == 1) {
                has_degree_one = true;
                break;
            }
        }

        if (has_degree_one) {
            cout << "--------------------------------------" << endl;
            cout << graphHeader << endl;
            cout << "Max Guaranteed Snake Length: 2 (Trivial: Degree 1 vertex found)" << endl;
            cout << "Execution Time: 0 ms" << endl;
            continue; // Skip the heavy calculations and move to the next graph
        }
        // ---------------------------------



        auto start = chrono::high_resolution_clock::now();
        memo_game.clear();
        int global_max = 0;

        for (int i = 0; i < N; i++) {
            vector<int> p; p.push_back(i);
            int res = solve_adversary(i, 1LL << i, p);
            if (res > global_max) global_max = res;
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> dur = end - start;

        cout << "--------------------------------------" << endl;
        cout << graphHeader << endl;
        cout << "Max Guaranteed Snake Length: " << global_max << endl;
        cout << "Execution Time: " << dur.count() << " ms" << endl;
    }
    return 0;
}