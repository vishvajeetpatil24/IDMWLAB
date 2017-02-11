#include <bits/stdc++.h>
using namespace std;
typedef vector<string> vstr;
int main()
{
	/*ifstream fp;
	fp.open("sample1.txt",ifstream::in);
	int n;
	fp >> n;
	string line;
	getline(fp,line);
	getline(fp,line);
	stringstream sin;
	sin <<line;
	string temp;
	while(sin >>temp)
	{
		cout <<temp<<" ";
	}
	cout <<"\n";*/
	ifstream fp;
	fp.open("FILE1.txt",ifstream::in);
	string line,temp;
	int tn,cnt1 = 0;
	fp >> tn;
	vstr lis;
	lis.resize(tn+1);
	getline(fp,line);
	getline(fp,line);
    string delimiter = ", ";
	size_t pos = 0;
	string token;
	while ((pos = line.find(delimiter)) != string::npos) 
	{
    	token = line.substr(0, pos);
    	lis[cnt1++] = token;
    	cout <<token<<" ";
    	line.erase(0, pos + delimiter.length());
	}
	lis[cnt1++] = line;
	cout <<line<<"\n";
	while(getline(fp,line))
	{
		stringstream sin;
		//cout <<line <<"\n";
		sin << line;
		int num;
		char ch;
		int count = 0;
		while(sin >>num)
		{
			sin >> ch;
			cout <<num<<" ";
		}
		cout <<"\n";
	}
	return 0;
}