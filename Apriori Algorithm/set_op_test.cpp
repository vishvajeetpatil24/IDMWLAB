#include <bits/stdc++.h>
using namespace std;
typedef set<int> iset;
int main()
{
	iset i,i1;
	int a[] = {4,5,6};
	int b[] = {4,7,8};
	int c[10] =  {0};
	i.insert(a,a+3);
	i1.insert(b,b+3);
	iset i2 = iset();
	set_difference(i.begin(),i.end(),i1.begin(),i1.end(),inserter(i2,i2.end()));
	cout <<i2.size();
	return 0;
}