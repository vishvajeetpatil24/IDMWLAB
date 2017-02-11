#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

int MIN_SUPPORT;
int MIN_CONFIDENCE;

map< int, vector<int> > dataset; //map to store dataset; map< TID, ITEMSET >
map< int, vector<int> > :: iterator mpit;
map< vector<int>, int> freq; //stores frequency of itemsets; map< ITEMSET, FREQUENCY >

/* Function to print dataset (transactions) */
void print_dataset()
{
	cout<<"Dataset\n";
	cout<<"TID ITEMSET\n";
	for(mpit = dataset.begin(); mpit != dataset.end(); mpit++){
		cout<<"[ "<<mpit->first<<" ]-->";
		for(int i = 0 ; i < mpit->second.size(); i++)
			cout<<mpit->second[i]<<" ";
		cout<<endl;
	}
}

/* Printing utility function */
void print( map< vector<int>, int> data ) // map< ITEMSET, FREQUENCY >
{
	vector<int> temp;
	map< vector<int>, int> :: iterator mit;
	cout<<"ITEMSET SUPPORT\n";
	for( mit = data.begin(); mit != data.end(); mit++){ //loop over data
		temp = mit->first;
		cout<<"[ ";
		for(int i = 0 ; i < (int)temp.size(); i++) //itemset
			cout<<temp[i]<<" ";
		cout<<"]-->"<<mit->second; //frequency of itemset
		if( mit->second < MIN_SUPPORT ) cout<<" (will be pruned)"; //pruned - while calculating Lk
		cout<<endl;
	}
}

/* Calculate C1 : possible candidate with 1-itemset */
void calculate_c1()
{
	vector<int> temp;
	for(mpit = dataset.begin(); mpit != dataset.end(); mpit++){
		for(int i = 0 ; i < mpit->second.size(); i++){
			temp.clear();
			temp.push_back(mpit->second[i]);
			freq[temp]++;
		}
	}
}

/* Calculate L1 : frequent 1-itemsets */
void calculate_l1()
{
	map< vector<int>, int> :: iterator mit;
	for( mit = freq.begin(); mit != freq.end(); mit++){
		if( mit->second < MIN_SUPPORT ){
			freq.erase(mit); //remove that itemset; support < min_support
		}
	}
}

int support(set<int> itemset)
{
	set< int> :: iterator it;
	int count, result = 0;
	for(mpit = dataset.begin(); mpit != dataset.end(); mpit++){
		count = 0;
		for(it = itemset.begin(); it != itemset.end(); it++)
			if( find( mpit->second.begin(), mpit->second.end(), *it) != mpit->second.end()) //check if itemset is present in the transaction
				count++;
		if( count == itemset.size())
			result++;
	}
	return result;
}

/* Calculate item set in Ck */
void calculate_ck(int k) 
{
	map< vector<int>, int> temp, result;
	map< vector<int>, int> :: iterator mp1, mp2;
	temp = freq;
	set<int> itemset;
	set<int> ::iterator it;
	for(mp1 = freq.begin() ; mp1 != freq.end(); mp1++){ //generating possible candidates for frequent itemset
		for(mp2 = temp.begin(); mp2 != temp.end(); mp2++){
			if( mp1->first == mp2->first ) continue; //continue for same itemset
			itemset.clear();
			itemset.insert(mp1->first.begin(), mp1->first.end() );
			itemset.insert(mp2->first.begin(), mp2->first.end() ); //generate the candidate itemset
			if( itemset.size() == k ){ // if no of item in itemset is k
				if( support(itemset) != 0 ){ //if candidate itemset is present in dataset (transactions)
					vector<int> tt(itemset.begin(), itemset.end());
					result[tt] = support(itemset); //add itemset to result ( Ck )
				}
			}
			//else
			//	cout<<"koi na\n";
		}
	}
	freq.clear();
	freq = result; //update freq with the current result ( Ck )
}

/* Calculate item set in Lk, using Ck */
bool calculate_lk(int k)
{
	map< vector<int>, int> result;
	map< vector<int>, int> :: iterator mp1;
	set<int> s1;
	for(mp1 = freq.begin() ; mp1 != freq.end(); mp1++){ //loop through for frequent itemset
		s1.clear();
		s1.insert(mp1->first.begin(), mp1->first.end());
		if( support(s1) >= MIN_SUPPORT ){ //check support of itemset
			result[mp1->first] = support(s1); //add itemset to result ( Lk )
		}
	}
	freq.clear();
	freq = result; //update freq with the current result ( Lk )
}

/* Process input from input file */
int processinput()
{
	string line;
	vector<int> entry;
	ifstream file;
	file.open("chess.dat");

	if( file.is_open() == false ){ //if there is error in opening the file
		cout<<"Error while opening input file"<<endl;
		return 0;
	}

	int count = 1, value, mul;
	//Reading file line by line
	while( getline(file, line)){
		entry.clear();
		for(int i = 0; i < (int)line.length(); i++){
			if( line[i] == ' ' ) continue; //escape black spaces
			value = 0, mul = 1;
			while( line[i+1] != ' ' && line[i+1] != '\n' && i < (int)line.length() ){ 
				value = value*mul + (line[i]-48); //convert charaster string to integer
				mul = mul*10;
				i++;
			}
			value = value*mul + (line[i] - 48);
			entry.push_back(value); //only bits are pushed (only for FILE1.txt and FILE2.txt)
		}
		dataset[count++] = entry;
	}
	file.close(); //close file
	return (count-1); //return no of transactions ; input file read successfully
}

int main()
{
	int total_transaction = processinput();
	int min_per;
	cout<<"Please enter minimum support % value ";
	cin>>min_per;
	MIN_SUPPORT = ceil(min_per*total_transaction)/100;
	//print_dataset(); //to print dataset
	static struct timeval tm1;
	//cout<<"\nC1\n";
	calculate_c1();
	//print(freq);

	//cout<<"\nL1\n";
	calculate_l1();
	//print(freq);
	
	int k = 2;
	while(1){ //make it while( k < some small value )
		calculate_ck(k);
		if( freq.size() == 0 ) {
			//cout<<"\nL"<<k<<"\nNo itemset\n";
			break;
		}
		//cout<<"\n\nC"<<k<<endl;
		//print(freq);

		calculate_lk(k);
		if( freq.size() == 0 ) {
			//cout<<"\nL"<<k<<"\nNo itemset\n";
			break;
		}
		//cout<<"\nL"<<k<<endl;
		//print(freq);
		k++;
	}
	struct timeval tm2;
    gettimeofday(&tm2, NULL);
    unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
    double ans = (double)t/10e9;
    cout <<ans<<"\n";
	return 0;
}
