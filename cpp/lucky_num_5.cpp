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

typedef unsigned long long ll;
typedef pair<int, int> pi;
typedef pair<pi, ll> ppi;
typedef map<pi, ll> mi;

#define c 1459
#define b 163
#define a 20


bool seive[1500];
int prime[300];

int dig[18];

int cnt_prime = 0;
int cnt_sum = 0;

mi* dp;
mi* dp1;


ll luck(ll x){
  int cnt = 0;
  while (x){
    dig[cnt] = x % 10;
    cnt++;
    x = x / 10;
  }

  ll res = 0;
  int cur_sum = 0;
  int cur_sqr = 0;

  mi::iterator it;
  pi px;
  for (int i = cnt - 1; i > 0; i--){
    fr (p, dig[i]){
      px = pi(cur_sum + p, cur_sqr + p * p);
      it = dp1[i].find(px);
      if (it != dp1[i].end())
	res += it->second;
    }
    cur_sum += dig[i];
    cur_sqr += dig[i] * dig[i];
  }

  for (int i = 0; i <= dig[0]; i++){
    if (seive[cur_sum + i] && seive[cur_sqr + i * i]){
      res++;
    }
  }

  return res;
}

int main(){
  
  fr (i, c){
    seive[i] = true;
  }
  
  seive[1] = false;
  seive[0] = false;
  cnt_prime = 0;

  f (i, 2, c){
    if (seive[i]){
      prime[cnt_prime] = i;
      cnt_prime++;
      for (int j = i * i; j < c; j += i){
	seive[j] = false;
      }
    }
  }

  int* temp = upper_bound(prime, prime + cnt_prime, 162);
  cnt_sum =(int) (temp - prime);
  //cout << cnt_sum << " " << prime[cnt_sum - 1] << endl;

  dp = new mi[19];
  dp1 = new mi[19];

  dp[0][pi(0, 0)] = 1;
  mi::iterator it, jt;
  f (m, 1, 19){
    f (i, 0, 10){
      for (it = dp[m - 1].begin(); it != dp[m - 1].end(); it++){
	pi p = pi((it->first).first + i, (it->first).second + i * i);
	jt = dp[m].find(p);
	if (jt != dp[m].end()){
	  jt->second += it->second;
	} else {
	  dp[m][p] = it->second;
	}
      }
    }
  }

  
  int p_sum_l, p_sqr_l, p_sum_h, p_sqr_h;
  f (m, 1, 18){
    for (it = dp[18 - m].begin(); it != dp[18 - m].end(); it++){
      pi px = it->first;
      ll cnt = 0;
      for (jt = dp[m].begin(); jt != dp[m].end(); jt++){
	if (seive[jt->first.first + px.first] && seive[jt->first.second + px.second])
	  cnt += jt->second;
      }
      dp1[m][px] = cnt;
    }
  }

   

  


  int t;
  s(t);
  ll x, y;
  while (t--){
    sll(x); sll(y);
    if (y == (ll)1000000000000000000){
      y--;
    }
    if (x == (ll)1000000000000000000){
      print("0\n");
      continue;
    }
    cout << y << " " << luck(y) << endl;
    cout << x << " " << luck(x - 1) << endl;
    printf("%lld\n", luck(y) - luck(x - 1));
  }


  return 0;
}
