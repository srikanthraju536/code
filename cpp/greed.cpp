// written by lonerdude(dvdreddy)
// all rights reserved
//the template by dvdreddy
#include <vector>
#include <queue>
#include <deque>
#include <map>
#include <iostream>
#include <cstring>
#include <string>
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;

#define s(x) scanf("%d",&x)
#define sll(x) scanf("%lld",&x)
#define sf(x) scanf("%lf",&x)
#define ss(x) scanf("%s",&x)

#define f(i,a,b) for(int i=a;i<b;i++)
#define fr(i,n)  f(i,0,n)

typedef long long ll;

typedef vector<double> VD;
typedef vector<VD> VVD;
typedef vector<int> VI;

double MinCostMatching(const VVD &cost, VI &Lmate, VI &Rmate) {
  int n = int(cost.size());

  // construct dual feasible solution
  VD u(n);
  VD v(n);
  for (int i = 0; i < n; i++) {
    u[i] = cost[i][0];
    for (int j = 1; j < n; j++) u[i] = min(u[i], cost[i][j]);
  }
  for (int j = 0; j < n; j++) {
    v[j] = cost[0][j] - u[0];
    for (int i = 1; i < n; i++) v[j] = min(v[j], cost[i][j] - u[i]);
  }
  
  // construct primal solution satisfying complementary slackness
  Lmate = VI(n, -1);
  Rmate = VI(n, -1);
  int mated = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (Rmate[j] != -1) continue;
      if (fabs(cost[i][j] - u[i] - v[j]) < 1e-10) {
	Lmate[i] = j;
	Rmate[j] = i;
	mated++;
	break;
      }
    }
  }
  
  VD dist(n);
  VI dad(n);
  VI seen(n);
  
  // repeat until primal solution is feasible
  while (mated < n) {
    
    // find an unmatched left node
    int s = 0;
    while (Lmate[s] != -1) s++;
    
    // initialize Dijkstra
    fill(dad.begin(), dad.end(), -1);
    fill(seen.begin(), seen.end(), 0);
    for (int k = 0; k < n; k++) 
      dist[k] = cost[s][k] - u[s] - v[k];
    
    int j = 0;
    while (true) {
      
      // find closest
      j = -1;
      for (int k = 0; k < n; k++) {
	if (seen[k]) continue;
	if (j == -1 || dist[k] < dist[j]) j = k;
      }
      seen[j] = 1;
      
      // termination condition
      if (Rmate[j] == -1) break;
      
      // relax neighbors
      const int i = Rmate[j];
      for (int k = 0; k < n; k++) {
	if (seen[k]) continue;
	const double new_dist = dist[j] + cost[i][k] - u[i] - v[k];
	if (dist[k] > new_dist) {
	  dist[k] = new_dist;
	  dad[k] = j;
	}
      }
    }
    
    // update dual variables
    for (int k = 0; k < n; k++) {
      if (k == j || !seen[k]) continue;
      const int i = Rmate[k];
      v[k] += dist[k] - dist[j];
      u[i] -= dist[k] - dist[j];
    }
    u[s] += dist[j];
    
    // augment along path
    while (dad[j] >= 0) {
      const int d = dad[j];
      Rmate[j] = Rmate[d];
      Lmate[Rmate[j]] = j;
      j = d;
    }
    Rmate[j] = s;
    Lmate[s] = j;
    
    mated++;
  }
  
  double value = 0;
  for (int i = 0; i < n; i++)
    value += cost[i][Lmate[i]];
  
  return value;
}

int dp[500][500];
int crd[500];
#define max 100000

int main(){
  int t;
  s(t);
  while (t--){
    int n;
    s(n);
    fr (i, n){
      s(crd[i]); crd[i]--;
    }
    fr (i, n){
      fr (j, n){
	if (i != j){
	  dp[i][j] = max;
	} else {
	  dp[i][i] = 0;
	}
      }
    }
    int m; s(m);
    fr (i, m){
      int x, y;
      s(x); s(y); x--; y--;
      dp[x][y] = 1;
      dp[y][x] = 1;
    }
    
    fr (p, n){
      fr (i, n){
	fr (j, n){
	  if (dp[i][p] + dp[p][j] < dp[i][j]){
	    dp[i][j] = dp[i][p] + dp[p][j];
	  }
	}
      }
    }
    
    VVD cst;
    cst.resize(n);
    fr (i, n){
      cst[i].resize(n);
    }
    
    fr (i, n){ 
      fr (j, n){
	cst[i][j] = (double) dp[crd[i]][j];
      }
    }
    
    VI lft; VI rt;

    double x = MinCostMatching(cst, lft, rt);
    int y = (int) x;
    printf("%d\n", y);
  }

  
  return 0;
}
