//Author - Vishvajeet Subhash Patil
//Note -- This program can be made to be more efficient than it is currently now both in time and memory ......... :)
#include <bits/stdc++.h>
using namespace std;
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
void setprint(itemset S)
{
	sit x;
	cout <<"{";
	for (x=S.begin();x!=S.end();x++)
	{
		cout <<*x<<",";
	}
	cout <<"}";
}
void rmaprint(rmap R)
{
	rit x;
	for (x=R.begin();x!=R.end();x++)
	{
		setprint(x->first);
		cout <<" --> ";
		setprint(R[x->first]);
	}
	cout <<"\n";
}
void imaprint(imap I)
{
	mit x;
	cout <<"-----------------------------------------\n";
	for (x=I.begin();x!=I.end();x++)
	{
		setprint(x->first);
		cout <<" --> ";
		cout <<I[x->first];
		cout <<"\n";
	}
}
int main()
{
	//Input for transaction DB T. I am assuming the data is already given in sorted format
	transaction T;
	ifstream fp;
	fp.open("chess.dat",ifstream::in);
	string line;
	rmap perma;
	imap current;
	superset L;
	while(getline(fp,line))
	{
		T.push_back(vec());
		stringstream sin;
		sin << line;
		int num;
		while(sin >>num)
		{
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
	cin >> minsup >> minconf;
	int numsup = (round)((double)T.size()*minsup);
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
	imaprint(current);
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
		imaprint(ncurrent);
		//rmaprint(nperma);
	}
	//Rules Generation -----------
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
						setprint(final);cout <<"-->";setprint(i1);cout<<"  C:"<<(double)(L[n-1][it->first])/L[k-1][final]<<"\n";
					}
				}
			}
		}
	}
	return 0;
}
