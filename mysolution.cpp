#include<bits/stdc++.h>
//first, let's get the 50.

using namespace std;
typedef pair<int, int> pii;
const int maxn = 5e5 + 30;

struct Bridges{
  vector<pii> adj[maxn];
  int dfsnum[maxn];
  int dfslow[maxn];
  pii dfspa[maxn];
  int tt = 0;
  vector<int> order;
  void dfs(int v){
    tt++; dfsnum[v] = tt; dfslow[v] = tt;
    order.push_back(v);
    int ct = 0;
    for(pii uu : adj[v]){
      int u = uu.first;
      if(u == dfspa[v].first){//THE PROBLEM WAS HERE: you DO need to reset that!
	ct++;
	if(ct == 1) continue;
      }
      if(dfsnum[u] == -1){
	dfspa[u] = pii(v, uu.second);
	dfs(u);
	dfslow[v] = min(dfslow[v], dfslow[u]);
      }
      else{
	dfslow[v] = min(dfslow[v], dfsnum[u]);
      }
    }
  }

  pii treepa[maxn];
  int depth[maxn];
  int treedepth[maxn];
  int redirect[maxn];
  int jump[maxn][21];
  int cc = 0;
  void finish(int r){
    assert(order[0] == r);
    cc++;
    assert(cc > 0);
    depth[cc] = 0;
    treedepth[cc] = 1;
    redirect[r] = cc;
    for(int i=0;i<21;i++) jump[cc][i] = 0;
    treedepth[0] = -1;
    
    for(int v : order){
      if(v == r) continue;
      if(dfslow[v] == dfsnum[v]){
	cc++;
	redirect[v] = cc;
	int p = redirect[dfspa[v].first];
	treepa[cc] = pii(p, dfspa[v].second);
	depth[cc] = depth[p] + treepa[cc].second;
	treedepth[cc] = treedepth[p] + 1;
	jump[cc][0] = p;
	for(int i=1;i<21;i++){
	  jump[cc][i] = jump[jump[cc][i-1]][i-1];
	}
      }
      else{
	redirect[v] = redirect[dfspa[v].first];
      }
    }
  }

  int LCA(int u, int v){
    if(treedepth[u] > treedepth[v]) swap(u,v);
    for(int i = 20; i >= 0; i--){
      if(treedepth[jump[v][i]] >= treedepth[u]) v = jump[v][i];
    }
    assert(treedepth[u] == treedepth[v]);
    if(u == v) return u;
    for(int i = 20; i >= 0; i--){
      if(jump[u][i] != jump[v][i]){
	u = jump[u][i]; v = jump[v][i];
      }
    }
    assert(jump[u][0] == jump[v][0]);
    return jump[u][0];
  }

  int unionpa[maxn];
  int rnk[maxn];
  int find(int x){
    if(x == unionpa[x]){
      return x;
    }
    int ans = find(unionpa[x]);
    unionpa[x] = ans;
    return ans;
  }
  void unite(int x, int y){
    int a = find(x);
    int b = find(y);
    if(a == b) return;
    if(rnk[a] > rnk[b]) swap(a,b);
    unionpa[a] = b;
    if(rnk[a] == rnk[b]) rnk[b]++;
  }
  
  void perform(vector<pair<pii, int> > edges, vector<int> & vv){
    vector<int> vtxs;
    for(int v : vv){
      vtxs.push_back(v);
    }
    for(auto x : edges){
      vtxs.push_back(x.first.first);
      vtxs.push_back(x.first.second);
    }
    for(int v : vtxs){
      adj[v].clear();
      dfsnum[v] = -1;
      dfspa[v] = pii(0,0);
      dfslow[v] = -1;
      unionpa[v] = v;
      rnk[v] = 0;
    }
    for(auto x : edges){
      int u = x.first.first;
      int v = x.first.second;
      int w = x.second;
      unite(u,v);
      adj[u].push_back(pii(v,w));
      adj[v].push_back(pii(u,w));
    }
    cc = 0;
    tt = 0;
    for(int v : vtxs){
      if(dfsnum[v] == -1){
	order.clear();
	dfs(v);
        finish(v);
      }
    }
  }
} Z, W;

bool cmp(int x, int y){
  return Z.redirect[x] < Z.redirect[y];
}

int main(){
  int n; cin >> n;
  int m; cin >> m;
  vector<pair<pii, int> > edges;
  for(int e=0;e<m;e++){
    int u, v;
    scanf("%d%d", &u, &v);
    assert(u != 0 && v != 0);
    assert(u <= n && v <= n);
    edges.push_back(make_pair(pii(u,v), 1));
  }
  
  vector<int> critical;
  for(int i=1;i<=n;i++) critical.push_back(i);
  Z.perform(edges, critical);
  

  int Q; cin >> Q;
  for(int q=0;q<Q;q++){
    int s, t; scanf("%d%d", &s, &t);
    int ne; scanf("%d", &ne);
    vector<pii> newedges;
    vector<pair<pii, int> > edgeshere;

    
    vector<int> vtxs;
    vtxs.push_back(s);
    vtxs.push_back(t);
    for(int g=0;g<ne;g++){
      int u, v; scanf("%d%d", &u, &v);
      vtxs.push_back(u);
      vtxs.push_back(v);
      newedges.push_back(pii(u,v));
    }
    sort(vtxs.begin(), vtxs.end());
    vtxs.resize(unique(vtxs.begin(), vtxs.end()) - vtxs.begin());
    sort(vtxs.begin(), vtxs.end(), cmp);

    vector<int> condensed;
    for(int i=1;i<vtxs.size();i++){
      if(Z.find(vtxs[i]) == Z.find(vtxs[i-1])){
	condensed.push_back(Z.LCA(Z.redirect[vtxs[i]], Z.redirect[vtxs[i-1]]));
      }
    }
    for(int v : vtxs){
      condensed.push_back(Z.redirect[v]);
    }

    sort(condensed.begin(), condensed.end());
    condensed.resize(unique(condensed.begin(), condensed.end()) - condensed.begin());
    
    vector<pair<pii, int> > edg;
    vector<int> anc;
    for(int v : condensed){
      while(!anc.empty() && Z.LCA(anc.back(), v) != anc.back()){
        anc.pop_back();
      }
      if(!anc.empty()) edg.push_back(make_pair(pii(anc.back(), v), Z.depth[v] - Z.depth[anc.back()]));
      anc.push_back(v);
    }

    for(pii x : newedges){
      edg.push_back(make_pair(pii(Z.redirect[x.first], Z.redirect[x.second]), 1));
    }
    for(auto x : edg){
      //cout << x.first.first << " " << x.first.second <<"    " << x.second << "?\n";
    }

    s = Z.redirect[s];
    t = Z.redirect[t];

    vector<int> cr;
    cr.push_back(s);
    cr.push_back(t);
    W.perform(edg, cr);


    if(W.find(s) != W.find(t)){
      cout << -1 << "\n";
    }
    else{
      int a = W.redirect[s];
      int b = W.redirect[t];
      int c = W.LCA(a,b);
      cout << W.depth[a] + W.depth[b] - 2 * W.depth[c] << "\n";
    }
  }
}
