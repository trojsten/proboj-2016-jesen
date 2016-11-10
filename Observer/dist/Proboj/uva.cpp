//house keeping stuuf
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <cctype>
#include <stack>
#include <bitset>
#include <queue>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <climits>
#include <cmath>

//shorcuts
#define For(i, n)for( int  i = 0;i<n;i++)
#define FOR(i,f,t)for( int i = f;i<t;i++)
#define RFor(i,f)for(int i = f;i>=0;i--)
#define RFOR(i,f,t)for(ll i = f;i>=t;i--)
#define Iter(i,a) for(decltype(a.begin()) i = a.begin();i != a.end();i++)
#define mp make_pair
#define range(c) c.begin(), c.end()
#define ff first
#define ss second
#define pf printf
#define ins insert
#define pb push_back
#define sz size
#define gcd __gcd
#define PI 3.1415926535897932384626433832795

//using fucking everything
using namespace std;

//more shortcuts
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<double ,double> pdd;
typedef pair<ll, ll> pll;
//one long shortcut 
typedef unsigned long long ull;

//too much performance
inline void magic_optimalization(){ios_base::sync_with_stdio(false);cin.tie(0);}
inline ll SQ(ll x){return x*x;}
inline ull USQ(ull x){return x*x;}
inline ll toInt(string s){stringstream ss(s);ll a; ss >> a;return a;}
inline string toStr(ll s){stringstream ss;ss << s;return ss.str();}
inline ll mod(ll n,ll m){return ((n % m)+m)%m;}

int n,m,dir=0;

void do_move(int &x, int &y){
	if(x+1 < n)x++;
	else x--;
}

int sc[5];

int main ()
{
	scanf("%d%d", &n, &m);
	pf("3\nMarcel 1 0 0\nEmo 0 1 0\nMiso 0 0 1\n%d %d\n", n, m);

	int x=n/2,y=m/2;
	For(i, 1000){
		pf("3\n");
		For(j, 3){
			sc[j] += rand() % 50;
			pf("%d %d %d %d %d\n", rand() % n, rand()%m, 0, 1, sc[j]);
		}

		For(j, n){
			For(k, m){
				int val = -1;
				if(x == j && y == k)val=0;
				pf("%d %d ", -1, val);
			}
			pf("\n");
		}

		do_move(x,y);
	}
	return 0;
}
