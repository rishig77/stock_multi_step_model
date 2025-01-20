#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define F first
#define S second
#define all(x) x.begin(), x.end()

class SegmentTree {
public:
    vector<ll> tree;
    ll size;

    SegmentTree(ll n) {
        size = n;
        tree.resize(4 * n, 0);
    }

    void update(ll idx, ll value, ll node, ll start, ll end) {
        if (start == end) {
            tree[node] += value;
        } else {
            ll mid = (start + end) / 2;
            if (idx <= mid) {
                update(idx, value, 2 * node, start, mid);
            } else {
                update(idx, value, 2 * node + 1, mid + 1, end);
            }
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    ll query(ll l, ll r, ll node, ll start, ll end) {
        if (r < start || l > end) {
            return 0;
        }
        if (l <= start && end <= r) {
            return tree[node];
        }
        ll mid = (start + end) / 2;
        ll leftQuery = query(l, r, 2 * node, start, mid);
        ll rightQuery = query(l, r, 2 * node + 1, mid + 1, end);
        return leftQuery + rightQuery;
    }

    void update(ll idx, ll value) {
        update(idx, value, 1, 1, size);
    }

    ll query(ll l, ll r) {
        return query(l, r, 1, 1, size);
    }
};

void solve(ll tc) {
    ll n, k;
    cin >> n >> k;

    set<ll> s;
    vector<ll> a(n);
    vector<pair<pair<int,int>,int>>rules(k);

    for (auto &x : a) {
        cin >> x;
        s.insert(x);
    }

    for (pair<pair<int,int>,int> &x : rules) {
        cin >> x.F.F >> x.F.S >> x.S;
        s.insert(x.F.F);
        s.insert(x.F.S);
    }

    vector<ll> cmpress(all(s));
    cmpress.insert(cmpress.begin(), 0);

    map<ll, ll> mp;
    for (int i = 1; i < (int)cmpress.size(); ++i) {
        mp[cmpress[i]] = i;
    }

    ll m = cmpress.size() - 1;
    SegmentTree segTree(m);

    multiset<ll> ms;

    for (auto &x : a) {
        x = mp[x];
        ms.insert(x);
    }

    for (pair<pair<int,int>,int> &x : rules) {
    	auto [l,r] = x.F;
        x.F.F = mp[l];
        x.F.S = mp[r];
    }

    ll ans = 0;

    sort(all(rules));

    for (auto [x,w] : rules) {
    	auto [l,r] = x;
    	ans -= w==1?(tc==2):0;
        if (segTree.query(l, r) < w) {
            ll need = w - segTree.query(l, r);
            ans += need;

            
            while (need--) {
                auto it = ms.upper_bound(r);
                it--;
                segTree.update(*it, 1);
                ms.erase(it);
            }
        }
    }

    cout << n - ans << endl;
}

int main() {
    ios_base::sync_with_stdio(false);  cin.tie(NULL), cout.tie(NULL);
    ll t;
    cin >> t;
    for (ll i = 1; i <= t; i++)
        solve(i);
}