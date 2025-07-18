//P1967 [NOIP 2013 提高组] 货车运输
/*并查集（路径压缩+按结点数合并）+Kruskal最小生成树+RMQ+前向星存图+倍增法LCA*/
#include <bits/stdc++.h>
#define endl '\n'
using namespace std;
using i64 = long long;
const int N = 1e4 + 9;
const int M = 5e4 + 9;
typedef pair<int, int> PII;

template <typename T>
bool cmin(T &a, const T &b) {
    return b < a ? a = b, 1 : 0;
}

template <typename T>
bool cmax(T &a, const T &b) {
    return b > a ? a = b, 1 : 0;
}

struct Edge0 {
    int u, v, w;
} edge1[M];
struct Edge {
    int next, to, w;
} edge[M];
int n, m, q;
int cnt = 1, deep[N], head[N], fa[N][21], w[N][21];
bool vis[N];

bool CMP(Edge0 &a, Edge0 &b) {
    return a.w > b.w;
}

struct DSU {
    vector<int> par, se;
    DSU(int n = 0) : par(n), se(n, 1) {
        iota(par.begin(), par.end(), 0);
    }
    int root(int x) {
        return par[x] = par[x] == x ? x : root(par[x]);
    }
    bool Merge(int x, int y) {
        x = root(x), y = root(y);
        if (x == y) return false;

        if (se[x] > se[y]) swap(x, y);

        par[x] = y;
        se[y] += se[x];
        return true;
    }
} dsu(N);

void addedge(int u, int v, int w) {
    edge[cnt].to = v;
    edge[cnt].w = w;
    edge[cnt].next = head[u];
    head[u] = cnt++;
}

void kruskal() {
    sort(edge1 + 1, edge1 + m + 1, CMP);
    for (int i = 1; i <= m; i++) {
        int u = edge1[i].u, v = edge1[i].v, w = edge1[i].w;
        if (dsu.Merge(u, v)) {
            addedge(u, v, w);
            addedge(v, u, w);
        }
    }
}

void dfs(int node) {
    vis[node] = true;
    for (int i = head[node]; i; i = edge[i].next) {
        int to = edge[i].to;
        if (vis[to]) continue;
        deep[to] = deep[node] + 1;
        fa[to][0] = node;
        w[to][0] = edge[i].w;
        dfs(to);
    }
    return;
}

int lca(int x, int y) {
    if (dsu.root(x) != dsu.root(y)) return -1;
    int res = INT_MAX;
    if (deep[x] > deep[y]) swap(x, y);
    for (int i = 20; i >= 0; i--) {
        if (deep[fa[y][i]] >= deep[x]) {
            cmin(res, w[y][i]);
            y = fa[y][i];
        }
    }
    if (x == y) return res;
    for (int i = 20; i >= 0; i--) {
        if (fa[x][i] != fa[y][i]) {
            cmin(res, min(w[x][i], w[y][i]));
            x = fa[x][i];
            y = fa[y][i];
        }
    }
    cmin(res, min(w[x][0], w[y][0]));
    return res;
}

void solve() {
    cin >> n >> m;

    for (int i = 1; i <= m; i++) {
        cin >> edge1[i].u >> edge1[i].v >> edge1[i].w;
    }
    kruskal();
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) {
            deep[i] = 1;
            fa[i][0] = i;
            w[i][0] = INT_MAX;
            dfs(i);
        }
    }
    for (int j = 1; j <= 20; j++) {
        for (int i = 1; i <= n; i++) {
            fa[i][j] = fa[fa[i][j - 1]][j - 1];
            w[i][j] = min(w[i][j - 1], w[fa[i][j - 1]][j - 1]);
        }
    }
    cin >> q;
    for (int i = 0; i < q; i++) {
        int x, y;
        cin >> x >> y;
        cout << lca(x, y) << endl;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // int t;
    // cin >> t;
    // while (t--)
    solve();

    return 0;
}   