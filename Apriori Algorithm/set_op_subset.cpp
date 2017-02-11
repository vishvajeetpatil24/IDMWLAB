#include <bits/stdc++.h>
using namespace std;
typedef set<int> se;
typedef se::iterator sit;
int main()
{
	int n = 3;
	set<int> s;
	int k[] = {12,53,6};
	vector<int> m = vector<int>(k,k+3);
	s.insert(m.begin(),m.end());
	unsigned int N = (1<<n);
	unsigned int v = 1;
	for (int j=0;j<1;j++)
	{
		v<<=1;
		v|=1;
	}
	while(v<N)
	{
		cout << v;
		unsigned int vt = v;
		sit S = s.begin(),si;
		se final;
		while(vt!=0)
		{
			if (vt&1)
			{
				final.insert(*S);
			}
			vt>>=1;
			++S;
		}
		unsigned int w; // next permutation of bits
		unsigned int t = v | (v - 1); // t gets v's least significant 0 bits set to 1
		// Next set to 1 the most significant bit to change, 
		// set to 0 the least significant ones, and add the necessary 1 bits.
		w = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1));
		v = w;
		cout <<"{";
		for (si=final.begin();si!=final.end();si++)
		{
			cout <<*si<<",";
		}
		cout <<"}";
	}
}