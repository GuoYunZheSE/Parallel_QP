#include<iostream>
#include<cstdio>
#include<queue>
#include<stack>
#include<string>
#include<cstring>
#include<cmath>
#include<vector>
#include<algorithm>
#include <set>
#include <map>
using namespace std;
#define ll long long
#define mem(a,b) memset(a,b,sizeof(a))
#define INF 1e9

const int maxn = 5000;
const int maxm = 1000;

//entry
struct node
{
	int x,y,val;
	node (int _x,int _y,int _val) : x(_x),y(_y),val(_val) {}
};

//valid entry for each row
vector <node> rows[maxm];

//valid entry for each column
vector <node> cols[maxn];

//column cover,row cover
int CC[maxn],CR[maxm];

//not chosen rows
set <int> T;

//threshold
double alpha = 0.08;
int rmin = 20,cmin = 20;

//num of rows and cols
int maxr,maxc;

//number of rows and cols which has 1 or more patterns covered
int rc = 0,cc = 0;

//max row/col cover ratio
int max_cover_row = 0,max_cover_col = 0;

//for shuffling
int rand_col[maxn];
int rand_row[maxm];

//compare two cols
struct cmpC
{
	bool operator () (int& a,int& b)
	{
		if(CC[a] == CC[b]) return rand_col[a] < rand_col[b];
		return CC[a] > CC[b];
	}
};

//compare two rows
struct cmpR
{
	bool operator () (int& a,int& b)
	{
		if(CR[a] == CR[b]) return rand_row[a] < rand_row[b];
		return CR[a] > CR[b];
	}
};

bool cmp(int a,int b)
{
	if(CR[a] == CR[b]) return rand_row[a] < rand_row[b];
	return CR[a] < CR[b];
}

//opsm pattern
struct OPSM
{
	set <int> base_row;
	set <int> killed_row; 
	set <int> chosen_row;
	set <int> chosen_col;
	vector <int> order;
	map <int,int> H;
	map <int,int> cnt;
	void reset()
	{
		base_row.clear();
		killed_row.clear();
		chosen_row.clear();
		chosen_col.clear();
		order.clear();
		H.clear();
		cnt.clear();
	}
};

vector <OPSM> opsms;

int pickcol(OPSM& opsm)
{
	//if opsm is empty
	if(opsm.chosen_col.size() == 0)
	{
		priority_queue <int,vector<int>,cmpC> q;
		for(set<int>::iterator i = T.begin();i != T.end();i++)
		{
			q.push(*i);
		}

		while(!q.empty())
		{
			int c = q.top();
			q.pop();
			priority_queue<int,vector<int>,cmpR> qr;
			for(vector<node>::iterator i = cols[c].begin();i != cols[c].end();i++)
			{
				qr.push(i->x);
				opsm.chosen_row.insert(i->x);
				opsm.cnt[i->x] = 1;
				opsm.H[i->x] = i->val;
			}
			if(qr.empty())
			{
				T.erase(c);
				continue;
			}
			opsm.base_row.insert(qr.top());
			opsm.chosen_row.insert(qr.top());
			opsm.chosen_col.insert(c);
			opsm.order.push_back(c);
			return c;
		}
		return -1;
	}


	priority_queue<int,vector<int>,cmpR> qr;
	for(set<int>::iterator i = opsm.chosen_row.begin();i != opsm.chosen_row.end();i++)
	{
		if(opsm.base_row.find(*i) != opsm.base_row.end()) continue;
		qr.push(*i);
	}

	while(!qr.empty())
	{
		int tr = qr.top();
		qr.pop();
		int sz = rows[tr].size();
		opsm.base_row.insert(tr);
		opsm.chosen_row.insert(tr);
	
		priority_queue<int,vector<int>,cmpC> qc;
		for(int i = 0;i < sz;i++)
		{
			node tmp = rows[tr][i];
			if(opsm.chosen_col.find(tmp.y) != opsm.chosen_col.end() || T.find(tmp.y) == T.end()) continue;
			if(tmp.val >= opsm.H[tr])
			{
				qc.push(tmp.y);
			}
		}

		if(qc.empty()) continue;
		int c = qc.top();
		opsm.chosen_col.insert(c);
		opsm.order.push_back(c);
		return c;
	}

	return -1;
}

int cal(OPSM opsm)
{
	int mrc = 0;
	vector<int> tmpR(opsm.chosen_row.begin(),opsm.chosen_row.end());
	for(vector<int>::iterator i = tmpR.begin();i != tmpR.end();i++)
	{
		mrc = max(CR[*i]+1,mrc);
	}
	int mcc = 0;
	vector<int> tmpC(opsm.chosen_col.begin(),opsm.chosen_col.end());
	for(vector<int>::iterator i = tmpC.begin();i != tmpC.end();i++)
	{
		mcc = max(CC[*i]+1,mcc);
	}
	return (mrc-max_cover_row)+(mcc-max_cover_col);
}
	


void addSuppSet(OPSM& opsm,int t)
{
	int mn = max((int)ceil(alpha*opsm.chosen_col.size()),1);
	for(vector<node>::iterator i = cols[t].begin();i != cols[t].end();i++)
	{
		int r = i->x;
		if(opsm.killed_row.find(r) != opsm.killed_row.end()) continue; 
		else
		{
			if(i->val >= opsm.H[r]) 
			{
				opsm.cnt[r] = opsm.cnt[r] + 1;
				opsm.H[r] = i->val;
				opsm.chosen_row.insert(r);
			}
			else 
			{
				opsm.chosen_row.erase(r);
				opsm.killed_row.insert(r);
			}
		}
	}

	vector<int> tmp;
	for(set<int>::iterator i = opsm.chosen_row.begin();i != opsm.chosen_row.end();i++)
	{
		if(opsm.cnt[*i] < mn) opsm.chosen_row.erase(*i);
		else tmp.push_back(*i);
	}
	

	sort(tmp.begin(),tmp.end(),cmp);
	int k = 0;
	for(vector<int>::iterator i = tmp.begin();i != tmp.end();i++)
	{
		k++;
		if(k > rmin && CR[*i]+1 > max_cover_row) opsm.chosen_row.erase(*i);
	}
}

int add(OPSM opsm)
{
	opsms.push_back(opsm);
	int co = 1;
	for(set<int> :: iterator i = opsm.chosen_row.begin();i != opsm.chosen_row.end();i++)
	{
		if(CR[*i] == 0) 
		{
			rc++;
			co = 0;
		}
		CR[*i]++;
		max_cover_row = max(max_cover_row,CR[*i]);
	}
	for(set<int> :: iterator i = opsm.chosen_col.begin(); i != opsm.chosen_col.end();i++)
	{
		if(CC[*i] == 0) 
		{
			cc++;
			co = 0;
		}
		CC[*i]++;
		max_cover_col = max(max_cover_col,CC[*i]);
	}
	return co;
}

void shuffle()
{
	srand(time(NULL));
	for(int i = 0;i <= maxc;i++) rand_col[i] = i;
	for(int i = 0;i < maxc;i++) swap(rand_col[i],rand_col[i+rand()%(maxc-i)]);
	for(int i = 0;i <= maxr;i++) rand_row[i] = i;
	for(int i = 0;i < maxr;i++) swap(rand_row[i],rand_row[i+rand()%(maxr-i)]);
}

void test1()
{
	freopen("in_u1.test","r",stdin);
	int x,y,val;
	int vis[maxm];
	mem(vis,0);
	vector<node> test_rows[maxm],test_cols[maxn];
	while(~scanf("%d%d%d",&x,&y,&val))
	{
		node tmp(x,y,val);
		test_rows[x].push_back(tmp);
		test_cols[y].push_back(tmp);
		vis[x] = 1;
	}
	int sz = opsms.size();
	double tprmse = 0;
	int cnt = 0;
	for(int i = 0;i < sz;i++)
	{
		for (set<int>::iterator j = opsms[i].chosen_row.begin();j != opsms[i].chosen_row.end();j++)
		{
			if(vis[*j])
			{
				OPSM opsm = opsms[i];
				int x = *j;
				double tmp[maxn];
				int tot = 0;
				int order_sz = opsm.order.size();
				for(int k = 0;k < order_sz;k++)
				{
					tmp[k] = -1;
					for(vector<node>::iterator p = rows[x].begin();p != rows[x].end();p++)
					{
						if(p->y == opsm.order[k]) tmp[k] = p->val;
						tot++;
					}
				}

				/*
				for(int k = 0;k < order_sz;k++)
				{
					if(tmp[k] == -1) continue;
					printf("%lf ",tmp[k]);
				}

				printf("\n");

				*/



				int tmpt[maxn];
				for(int k = 0;k < order_sz;k++)
				{
					tmpt[k] = -1;
					for(vector<node>::iterator p = test_rows[x].begin();p != test_rows[x].end();p++)
					{
						if(p->y == opsm.order[k]) 
						{
							tmpt[k] = p->val;
							tot++;
						}
					}
				}

				double l = -1,r = -1;
				int num = 0;
				for(int p = 0;p < order_sz;p++)
				{
					if((tmp[p]+1) < 1e-6)
					{
						num++;
						continue;
					}
					r = tmp[p];
					if(l == -1)
					{
						for(int q = p;q >= 0;q--) tmp[q] = r;
						l = r;
						num = 0;
					}
					else 
					{
						for(int q = 1;q <= num;q++)
						{
							tmp[p-num-1+q] = (double)(abs(r-l)/(num+1)*q);
						}
						l = r;
						num = 0;
					}
				}
				int q = order_sz-1;
				while(tmp[q] == -1) 
				{
					tmp[q] = l;
					q--;
				}

				double prmse = 0;
				int tcnt = 0;
				for(int p = 0;p < order_sz;p++)
				{
					if(tmpt[p] != -1) 
					{
						tcnt++,prmse += (tmp[p]-tmpt[p])*(tmp[p]-tmpt[p]);
					}
				}
				if(tcnt != 0) prmse /= tcnt;
				prmse = sqrt(prmse);

				tprmse += prmse;
				cnt++;
			}
		}
		//printf("%lf %d\n",tscore,cnt);
	}
	printf("P-RMSE: %lf\n",tprmse/cnt);

}


void test2()
{
	freopen("in_u1.test","r",stdin);
	int x,y,val;
	int vis[maxm];
	mem(vis,0);
	while(~scanf("%d%d%d",&x,&y,&val))
	{
		node tmp(x,y,val);
		rows[x].push_back(tmp);
		cols[y].push_back(tmp);
		vis[x] = 1;
	}
	int sz = opsms.size();
	double tscore = 0;
	int cnt = 0;
	for(int i = 0;i < sz;i++)
	{
		for (set<int>::iterator j = opsms[i].chosen_row.begin();j != opsms[i].chosen_row.end();j++)
		{
			if(vis[*j])
			{
				int x = *j;
				int tmp[maxn];
				int tot = 0;
				for(vector<int>::iterator k = opsms[i].order.begin();k != opsms[i].order.end();k++)
				{
					for(vector<node>::iterator p = rows[x].begin();p != rows[x].end();p++)
					{
						if(p->y == *k) tmp[tot++] = p->val;
					}
				}
				int diff = 0;
				for(int a = 0;a < tot;a++)
				{
					for(int b = a+1;b < tot;b++)
					{
						if(tmp[a] > tmp[b]) diff++;
					}
				}
				//tscore += diff*2.0/opsms[i].order.size()/(opsms[i].order.size()+1);
				tscore += diff*2.0/tot/(tot+1);
				//printf("%lf\n",diff*2.0/opsms[i].order.size()/(opsms[i].order.size()+1));
				cnt++;
			}
		}
		//printf("%lf %d\n",tscore,cnt);
	}
	printf("K-score: %lf\n",tscore/cnt);
}

int main()
{
	int x,y,val;
	maxr = maxc = 0;
	int cnt = 0;
	freopen("in_u1.base","r",stdin);
//	freopen("out.txt","w",stdout);
	while(~scanf("%d%d%d",&x,&y,&val))
	{
		if(x >= maxm || y >= maxn) continue;
		node tmp(x,y,val);
		maxr = max(maxr,x);
		maxc = max(maxc,y);
		rows[x].push_back(tmp);
		cols[y].push_back(tmp);
	}
	clock_t start = clock();
	double addTime = 0;
	double suppTime = 0;
	double calTime = 0;
	int converge = 0;
	int iter_cnt = 0;
	while(!converge)
	{
		OPSM opsm;
		opsm.reset();
		converge = 1;
		int added = 0;
		for(int i = 0;i <= maxc;i++) T.insert(i);
		shuffle();
		while(T.size() != 0)
		{
			iter_cnt++;
			clock_t start_add = clock();
			int t = pickcol(opsm);
			addTime += (double)(clock()-start_add)/CLOCKS_PER_SEC;
			if(t == -1)
			{
				if(opsm.chosen_row.size() >= rmin && opsm.chosen_col.size() >= cmin)
				{
					printf("added: %d %d\n",opsm.chosen_row.size(),opsm.chosen_col.size());
					if(add(opsm) == 0) converge = 0;
					added = 1;
				}
				opsm.reset();
			}
			else
			{
				T.erase(t);
				if(opsm.chosen_col.size() == 1)
				{
					continue;
				}
				OPSM tmp = opsm;
				clock_t start_supp = clock();
				addSuppSet(tmp,t);
				double tt = (double)(clock()-start_supp)/CLOCKS_PER_SEC;
				suppTime += tt;
				if(opsm.chosen_row.size() >= rmin && opsm.chosen_col.size() >= cmin && cal(opsm) < cal(tmp))
				{
					if(add(opsm) == 0) converge = 0;
					added = 1;
					opsm.reset();
				}
				else if (tmp.chosen_row.size() >= rmin)
				{
					opsm = tmp;
				}

				if(T.empty() && opsm.chosen_row.size() >= rmin && opsm.chosen_col.size() >= cmin)
				{
					if(add(opsm) == 0) converge = 0;
					added = 1;
				}
			}
		}
		clock_t start_cal = clock();
		calTime += (double)(clock()-start_cal)/CLOCKS_PER_SEC;
		if (!added) converge = 0;
	}

	printf("Total time: %lf\n",(double)(clock()-start)/CLOCKS_PER_SEC);
	printf("testing\n");

	test1();
	test2();

	int tmp = opsms.size();
	printf("Num of patterrns: %d\n",tmp);
//	for(int i = 0;i < tmp;i++) printf("%d %d\n",opsms[i].chosen_row.size(),opsms[i].chosen_col.size());
	//for(int i = 0;i < maxr;i++) printf("%d\n",CR[i]);
	printf("Coverage ratio: %lf %lf\n",(double)rc/(1+maxr),(double)cc/(1+maxc));
	printf("Overlapping degree: %d %d\n",max_cover_row,max_cover_col);
	printf("Iteration time: %d\n",iter_cnt);
	return 0;
}
