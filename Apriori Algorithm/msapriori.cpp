#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vec;
typedef vector<vec> transaction;
typedef pair<int,double> pd;
typedef vector<pd> mis;
mis suparr;
typedef map<int,int> loc;
loc loctable;
class compfu
{
public:
	bool operator()(const int &x,const int &y) const
	{
		return loctable[x]<loctable[y];
		return x<y;
	}
};
typedef set<int> rowset;
typedef set<int,compfu> itemset;
typedef map<itemset,rowset> iim;
typedef set<itemset> superset1;
typedef vector<iim> superset;
typedef superset1::iterator suit;
typedef iim::iterator iit;
typedef itemset::iterator sit;
typedef rowset::iterator rit;
void setprint(itemset I)
{
	sit x;
	cout <<"{";
	for (x=I.begin();x!=I.end();x++)
	{
		cout <<*x<<" ";
	}
	cout <<"}";
}
void rowprint(rowset I)
{
	rit x;
	cout <<"{";
	for (x=I.begin();x!=I.end();x++)
	{
		cout <<*x<<" ";
	}
	cout <<"}";
}
void iimprint(iim M)
{
	iit x;
	for (x=M.begin();x!=M.end();x++)
	{
		setprint(x->first);
		cout <<" --> ";
		rowprint(x->second);
		cout <<"\n";
	}
}
bool cmp(const pd &x,const pd &y)
{
	return (y.second-x.second)>10e-6;
}
int main()
{
	double minconf;
	superset L;
	transaction T;
	ifstream fp;
	fp.open("sample.dat",ifstream::in);
	string line;
	getline(fp,line);
	stringstream sap;
	sap << line;
	int num1;
	double sup1;
	iim perma;
	while(sap >> num1 >> sup1)
	{
		suparr.push_back(pd(num1,sup1));
	}
	sort(suparr.begin(),suparr.end(),cmp);
	for (int j=0;j<suparr.size();j++)
	{
		loctable[suparr[j].first] = j;
	}
	loc::iterator ma;
	/*for (ma=loctable.begin();ma!=loctable.end();ma++)
	{
		cout <<ma->first<<" "<<loctable[ma->first]<<" ";
	}
	cout <<"\n";*/
	int cnt = 0;
	iit pit;
	loc fcount;
	while(getline(fp,line))
	{
		T.push_back(vec());
		stringstream sin;
		sin << line;
		int num;
		while(sin >>num)
		{
			T[T.size()-1].push_back(num);
			itemset S;
			S.insert(num);
			if (perma.find(S)==perma.end())
			{
				perma[S] = rowset();
			}
			perma[S].insert(cnt);
			if (fcount.find(num)==fcount.end())
			{
				fcount[num] = 0;
			}
			fcount[num] = fcount[num] + 1;
		}
		cnt++;
	}
	cin >> minconf;
	loc numsuparr;
	for (int j=0;j<suparr.size();j++)
	{
		numsuparr[suparr[j].first] = round((double)T.size()*suparr[j].second);
	}
	iimprint(perma);
	//For first pass process
	iit x;
	itemset F1;
	int ans = -1;
	for(x=perma.begin();x!=perma.end();x++)
	{
		sit a;
		a = (x->first).begin();
		int num = *a;
		if (perma[x->first].size()>=numsuparr[num])
		{
			if (ans==-1)
			{
				ans = num;
			}
			else if (loctable[num]<loctable[ans])
			{
				ans = num;
			}
		}
	}
	cnt = loctable[ans];
	//cout <<cnt<<"\n";
	while(cnt<suparr.size())
	{
		if (fcount[suparr[cnt].first]>=numsuparr[ans])
		{
			F1.insert(suparr[cnt].first);
		}
		if (fcount[suparr[cnt].first]<numsuparr[ans])
		{
			itemset x;
			x.insert(suparr[cnt].first);
			perma.erase(x);
		}
		cnt++;
	}
	sit a;
	for (a=F1.begin();a!=F1.end();a++)
	{
		itemset x;
		x.insert(*a);
		if (perma[x].size()<numsuparr[*a])
		{
			//cout <<*a<<"\n";
			perma.erase(x);
		}
	}
	L.push_back(perma);
	//setprint(F1);
	cout <<".............................................\n";
	iimprint(perma);
	for (int k = 2;perma.size()!=0;k++)
	{
		iim nperma;
		superset1 combi;
		if (k==2)
		{
			//2level candidate generation
			sit a,a1;
			for (a=F1.begin();a!=F1.end();a++)
			{
				if(fcount[*a]>=numsuparr[*a])
				{
					a1 = a;
					a1++;
					for (;a1!=F1.end();a1++)
					{
						if (fcount[*a1]>=numsuparr[*a1])
						{
							itemset a2,a4,a5;
							a2.insert(*a);
							a2.insert(*a1);
							a4.insert(*a);
							a5.insert(*a1);
							rowset i1;
							set_intersection(perma[a4].begin(),perma[a4].end(),perma[a5].begin(),perma[a5].end(),inserter(i1,i1.end()));
							nperma[a2] = i1;
							combi.insert(a2);
						}
					}
				}
			}
		}
		else
		{
			iit it,it1;
			int cnt = 0;
			for (it=perma.begin();cnt<(perma.size()-1);it++)
			{
				cnt++;
				iit x = it;
				for (it1=++x;it1!=perma.end();it1++)
				{
					itemset i = it->first;
					rowset i1;
					i.insert((it1->first).begin(),(it1->first).end());
					if (i.size()==k)
					{
						set_intersection(perma[it->first].begin(),perma[it->first].end(),perma[it1->first].begin(),perma[it1->first].end(),inserter(i1,i1.end()));
						nperma[i] = i1;
						combi.insert(i);
					}
				}
			}
			//The pruning Step .....
			suit xl;
			for (xl=combi.begin();xl!=combi.end();xl++)
			{
				itemset i1 = *xl;
				int n = i1.size();
				sit lap;
				lap = i1.begin();
				int n1,n2;
				n1 = *(lap++);
				n2 = *lap;
				unsigned int N = (1<<n);
				unsigned int v = 1;
				for (int la=0;la<n-2;la++)
				{
					v<<=1;
					v|=1;
				}
				while(v<N)
				{
					itemset final;
					sit S = i1.begin();
					unsigned int vt = v;
					while(vt!=0)
					{
						if (vt&1)
						{
							final.insert(*S);
						}
						vt>>=1;
						++S;
					}
					if ((v&1)||(numsuparr[n1]==numsuparr[n2]))
					{
						if (perma.find(final)==perma.end())
						{
							nperma.erase(*xl);
							combi.erase(*(xl++));
						}
					}
					unsigned int w; // next permutation of bits
					unsigned int t = v | (v - 1); // t gets v's least significant 0 bits set to 1
					// Next set to 1 the most significant bit to change, 
					// set to 0 the least significant ones, and add the necessary 1 bits.
					w = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(v) + 1));
					v = w;
				}
			}
		}
		cout <<".............................................\n";
		perma = nperma;
		suit xl;
		for (xl=combi.begin();xl!=combi.end();xl++)
		{
			//setprint(*xl);
			itemset i3 = *xl;
			sit ii3 = i3.begin();
			int n4 =  *ii3;
			//cout <<n4<<" ";
			if (perma[*xl].size()<numsuparr[n4])
			{
				perma.erase(*xl);
			}
		}
		L.push_back(perma);
		iimprint(perma);
	}
	iit it;
	for (int j=1;j<L.size();j++)
	{
		for (it=L[j].begin();it!=L[j].end();it++)
		{
			int n = (it->first).size();
			//cout << n <<"\n";
			//setprint(it->first);
			int numconf = (round)((double)(L[n-1][it->first]).size()/minconf);
			unsigned int N = (1<<n);
			for (int k=1;k<n;k++)
			{
				unsigned int v = 1;
				for (int l=0;l<k-1;l++)
				{
					v<<=1;
					v|=1;
				}
				while(v<N)
				{
					itemset final;
					sit S = (it->first).begin();
					unsigned int vt = v;
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
					if (L[k-1][final].size()<=numconf)
					{
						itemset i1;
						set_difference((it->first).begin(),(it->first).end(),final.begin(),final.end(),inserter(i1,i1.end()));
						setprint(final);cout <<"-->";setprint(i1);cout<<"  C:"<<(double)(L[n-1][it->first]).size()/L[k-1][final].size()<<"\n";
					}
				}
			}
		}
	}
	return 0;
}