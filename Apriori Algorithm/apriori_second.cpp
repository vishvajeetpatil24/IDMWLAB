//Author - Vishvajeet Subhash Patil
//Note -- This program can be made to be more efficient than it is currently now both in time and memory ......... :)
#include <bits/stdc++.h>
using namespace std;
typedef vector<string> vstr;
typedef vector<int> vec;
typedef vector<vec> transaction;
typedef set<int> itemset;
typedef pair<itemset,int> pa;
typedef map<itemset,int> imap;
typedef vector<imap> superset; 
typedef map<itemset,itemset> rmap;
typedef rmap::iterator rit;
typedef imap::iterator mit;
typedef itemset::iterator sit;
void setprint(itemset S,vstr lis)
{
	sit x;
	cout <<"{";
	for (x=S.begin();x!=S.end();x++)
	{
		cout <<lis[*x]<<",";
	}
	cout <<"}";
}
void dsetprint(itemset S)
{
	sit x;
	cout <<"{";
	for (x=S.begin();x!=S.end();x++)
	{
		cout <<*x<<",";
	}
	cout <<"}";
}
void rmaprint(rmap R,vstr lis)
{
	rit x;
	for (x=R.begin();x!=R.end();x++)
	{
		setprint(x->first,lis);
		cout <<" --> ";
		dsetprint(R[x->first]);
	}
	cout <<"\n";
}
void imaprint(imap I,vstr lis)
{
	mit x;
	//cout <<"-----------------------------------------\n";
	for (x=I.begin();x!=I.end();x++)
	{
		setprint(x->first,lis);
		cout <<" --> ";
		cout <<I[x->first];
		cout <<"\n";
	}
}
void apriori(imap current,rmap perma,double minsup,double minconf,int Tsize,superset L,vstr lis)
{
	int numsup = (round)((double)Tsize*minsup);
	mit it,it1;
	sit its;
	for (it=current.begin();it!=current.end();)
	{
		if (current[it->first]<numsup)
		{
			perma.erase(it->first);
			current.erase((it++)->first);
		}
		else
		{
			++it;
		}
	}
	L.push_back(current);
	cout <<".........................Frequent Itemsets..........................\n";
	imaprint(current,lis);
	//rmaprint(perma);
	//For other levels automated procedure
	int level = 2;
	imap oldlevel = current;
	while(current.size()>1)
	{
		imap ncurrent;
		rmap nperma;
		int cnt = 0;
		for (it=current.begin();cnt<(current.size()-1);it++)
		{
			cnt++;
			mit x = it;
			for (it1=++x;it1!=current.end();it1++)
			{
				itemset i = it->first,i1;
				i.insert((it1->first).begin(),(it1->first).end());
				if (i.size()==level)
				{
					set_intersection(perma[it->first].begin(),perma[it->first].end(),perma[it1->first].begin(),perma[it1->first].end(),inserter(i1,i1.end()));
					if (i1.size()>=numsup)
					{
						ncurrent[i] = i1.size();
						nperma[i] = i1;
					}
				}
			}
		}
		level++;
		oldlevel = current;
		current = ncurrent;
		perma = nperma;
		L.push_back(ncurrent);
		imaprint(ncurrent,lis);
		//rmaprint(nperma);
	}
	//Rules Generation -----------
	cout <<".........................Generated Rules..........................\n";
	for (int j=1;j<L.size();j++)
	{
		for (it=L[j].begin();it!=L[j].end();it++)
		{
			int n = (it->first).size();
			//setprint(it->first);
			int numconf = (round)((double)(L[n-1][it->first])/minconf);
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
					if (L[k-1][final]<=numconf)
					{
						itemset i1;
						set_difference((it->first).begin(),(it->first).end(),final.begin(),final.end(),inserter(i1,i1.end()));
						setprint(final,lis);cout <<"-->";setprint(i1,lis);cout<<"  C:"<<(double)(L[n-1][it->first])/L[k-1][final]<<"\n";
					}
				}
			}
		}
	}
}
int main()
{
	//Input for transaction DB T. I am assuming the data is already given in sorted format
	transaction T;
	ifstream fp;
	fp.open("FILE1.txt",ifstream::in);
	string line,temp;
	rmap perma;
	imap current;
	superset L;
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
    	line.erase(0, pos + delimiter.length());
	}
	lis[cnt1++] = line;
	while(getline(fp,line))
	{
		stringstream sin;
		T.push_back(vec());
		sin << line;
		int num;
		char ch;
		int count = 0;
		while(sin >>num)
		{
			sin >> ch;
			if (num==1)
			{
				num = count;
				count++;
			}
			else
			{
				count++;
				continue;
			}
			T[T.size()-1].push_back(num);
			itemset i;
			i.insert(num);
			if (perma.find(i)==perma.end())
			{
				perma[i] = itemset();
				perma[i].insert(T.size()-1);
			}
			else
			{
				perma[i].insert(T.size()-1);
			}
			if (current.find(i)==current.end())
			{
				current[i] = 1;
			}
			else
			{
				current[i]++;
			}
		}
	}
	//Input minsup and minconf
	double minsup,minconf;
	minsup = 0.1;
	//cin >> minsup >> minconf;
	//rmaprint(perma,lis);
	for (int j=0;j<8;j++,minsup+=0.1)
	{
		minconf = 0.1;
		for (int k=0;k<6;k++,minconf+=0.1)
		{
			cout <<"Apriori Algorithm with minimum Support "<<minsup<<" and minimum Confidence "<<minconf<<"\n";
			apriori(current,perma,minsup,minconf,T.size(),L,lis);
			cout <<"\n";
		}
	}
	return 0;
}
