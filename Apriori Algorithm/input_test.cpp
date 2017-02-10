#include <bits/stdc++.h>
using namespace std;
int main()
{
	ifstream fp;
	fp.open("sample.txt",ifstream::in);
	string line;
	while(getline(fp,line))
	{
		stringstream sin;
		sin << line;
		int num;
		char ch;
		while(sin >>num)
		{
			sin >> ch;
			cout << num <<" ";
		}
		cout <<"\n";
	}
		return 0;
}