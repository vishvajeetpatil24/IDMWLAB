//Author - Vishvajeet Subhash Patil
//The FP Growth Approach
#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vec;
typedef vector<vec> transaction;
typedef map<int,int> nmap;
typedef set<int> itemset;
typedef map<itemset,int> cmap;
typedef map<int,cmap> fmap;
typedef cmap::iterator cit;
typedef itemset::iterator sit;
typedef fmap::iterator fit;
#define inf 111110
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
struct node
{
	map<int,node*> next;
	node* back;
	int value;
	int counter;
	node()
	{
		back = NULL;
		value = -inf;
		counter = 1;
	}
	node(int v,node *par)
	{
		back = par;
		value = v;
		counter = 1;
	}
	node(int v,node *par,int inc)
	{
		back = par;
		value = v;
		counter = inc;
	}
};
typedef vector<node*> nlist;
typedef map<int,node*> smap;
typedef map<int,nlist> tdata;
typedef tdata::iterator tdit;
typedef smap::iterator smit;
typedef nmap::iterator nit;
class tree
{
	public:
		node head;
		tdata info;
		node* curb;
		tree()
		{
			head = node();
			curb = &head;
		}
		void add(int x)
		{
			if ((curb->next).find(x)==(curb->next).end())
			{
				node *temp = new node(x,curb);
				curb->next[x] = temp;
				curb = temp;
				info[x].push_back(temp);
			}
			else
			{
				node *temp = (curb->next)[x];
				temp->counter = temp->counter + 1;
				curb = temp;
				//cout <<temp->counter<<" "<<x<<"\n";
				//info[x].push_back(temp);
			}
		}
		void add_c(int x,int inc)
		{
			if ((curb->next).find(x)==(curb->next).end())
			{
				node *temp = new node(x,curb,inc);
				curb->next[x] = temp;
				curb = temp;
				info[x].push_back(temp);
			}
			else
			{
				node *temp = (curb->next)[x];
				temp->counter = temp->counter + inc;
				curb = temp;
				//cout <<temp->counter<<" "<<x<<"\n";
				//info[x].push_back(temp);
			}
		}
		void reset()
		{
			curb = &head;
		}
};
void fmaprint(fmap x)
{
	//fmap is map from int to another map<itemset,int>
	fit it;
	for (it=x.begin();it!=x.end();it++)
	{
		cmap y = x[it->first];
		cit it1;
		for (it1=y.begin();it1!=y.end();it1++)
		{
			setprint(it1->first);
			cout <<" --> ";
			cout <<(it1->second)<<"\n";
		}
	}
}
fmap L;
nmap guide;
bool cmp(const int &x,const int &y)
{
	return guide[x]>guide[y];
}
void tdataprint(tdata x)
{
	tdit it;
	for (it=x.begin();it!=x.end();it++)
	{
		cout <<it->first<<"		---		";
		for (int j=0;j<x[it->first].size();j++)
		{
			cout <<x[it->first][j]->counter<<" "; 
		}
		cout <<"\n";
	}
}
int numsup = -1;


void fpgrowth(tree st,itemset init)
{
	//Formation of Frequent pattern bases
	tdit it;
	tdata inx = st.info;
	for (it=inx.begin();it!=inx.end();it++)
	{
		itemset init1 = init;
		init1.insert(it->first);
		int cur = it->first;
		nlist X = (st.info)[cur];
		int sum = 0;
		for (int j=0;j<X.size();j++)
		{
			sum += X[j]->counter;
		}
		(L[init1.size()])[init1] = sum;
		transaction T1;
		vec counts;
		tree st1 = tree();
		nmap guide1;
		for (int j=0;j<X.size();j++)
		{
			T1.push_back(vec());
			stack<int> S;
			node* temp = X[j]->back;
			while((temp->value)!= -inf)
			{
				S.push(temp->value);
				temp = temp->back;
			}
			while(!S.empty())
			{
				T1[T1.size()-1].push_back(S.top());
				int num = S.top();
				if (guide1.find(num)==guide1.end())
				{
					guide1[num] = X[j]->counter;
				}
				else
				{
					guide1[num] = guide1[num] + X[j]->counter;
				}
				S.pop();
			}
			counts.push_back(X[j]->counter);
		}
		for (int j=0;j<T1.size();j++)
		{
			st1.reset();
			for (int k=0;k<T1[j].size();k++)
			{
				if(guide1[T1[j][k]]>=numsup)
				{
					st1.add_c(T1[j][k],counts[j]);
				}
			}
		}
		fpgrowth(st1,init1);
	}
	//tdataprint(st1.info);
	/*
	for (int j=0;j<T1.size();j++)
	{
		for (int k=0;k<T1[j].size();k++)
		{
			cout <<T1[j][k]<<" ";
		}
		cout <<"  -----  ";
		cout <<counts[j];
		cout <<"\n";
	}*/
}
int main()
{
	//Input for transaction DB T. I am assuming the data is already given in sorted format
	double minsup ,minconf;
	cin >> minsup >> minconf;
	transaction T;
	ifstream fp;
	fp.open("chess.dat",ifstream::in);
	string line;
	while(getline(fp,line))
	{
		T.push_back(vec());
		stringstream sin;
		sin << line;
		int num;
		while(sin >>num)
		{
			T[T.size()-1].push_back(num);
			if (guide.find(num)==guide.end())
			{
				guide[num] = 1;
			}
			else
			{
				guide[num] = guide[num] + 1;
			}
		}
	}
	numsup = (round)((double)T.size()*minsup);
	//Sorting step complete
	for (int j=0;j<T.size();j++)
	{
		sort(T[j].begin(),T[j].end(),cmp);
	}
	//Tree creation and element obtaining
	tree st = tree();
	//int cur = -1,tval = INT_MAX;
	for (int j=0;j<T.size();j++)
	{
		st.reset();
		for (int k=0;k<T[j].size();k++)
		{
			if(guide[T[j][k]]>=numsup)
			{
				st.add(T[j][k]);
			}
		}
	}
	itemset init = itemset();
	fpgrowth(st,init);
	fmaprint(L);
	cout <<".........................Generated Rules..........................\n";
	fit itl;
	for (itl=L.begin();itl!=L.end();itl++)
	{
		int j = itl->first;
		cit it;
		for (it=L[j].begin();it!=L[j].end();it++)
		{
			int n = (it->first).size();
			//setprint(it->first);
			int numconf = (round)((double)(L[n][it->first])/minconf);
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
					if (L[k][final]<=numconf)
					{
						itemset i1;
						set_difference((it->first).begin(),(it->first).end(),final.begin(),final.end(),inserter(i1,i1.end()));
						setprint(final);cout <<"-->";setprint(i1);cout<<"  C:"<<(double)(L[n][it->first])/L[k][final]<<"\n";
					}
				}
			}
		}
	}
	//tdataprint(st.info);
	return 0;
}
