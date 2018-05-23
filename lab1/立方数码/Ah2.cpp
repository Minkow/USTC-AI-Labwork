#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath> 
#include <cstdlib>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <time.h>
#include <algorithm>

int target[27];

using namespace std;

vector<int>position(27);

enum actions{
	U,D,L,R,F,B
};

struct state
{
	vector<int> map;
	int g; // cost
	int h; //h value 
	string path;
	
	void setnum(int array[27])
	{
		for(int i=0;i<27;i++)
			map.push_back(array[i]);
	}
	
	bool operator <(const state &x) const
	{
		return g+h>x.g+x.h;
	}
}end;

int getzero(state x)
{
	for(int i=0;i<27;i++)
	{
		if(x.map[i]==0)
			return i;
	}
}

int h2(int x, int y)
{
	int h=0;
	h+=abs(x/9-y/9)+((x%9/3)-(y%9)/3)+abs(x%3-y%3);
	return h;
}

int tryswap(int now, int next, int index)
{
	int t=0;
	t-=h2(now,position[index]);
	t+=h2(next,position[index]);
	return t;
}

priority_queue<state> open;//优先队列维护open集(便于取最小值 
set<vector<int> > close;//set集合维护close(便于查重 

string astar(state s)
{
	open.push(s);
	int x;
	char action;
	while (!open.empty())
	{
		s=open.top();
		open.pop();
		if (close.find(s.map)==close.end())
		{
			close.insert(s.map);
			if(s.map == end.map)
			{
				return s.path;
			}
			int x=getzero(s);
			for(int i=0;i<6;i++)
			{
				int t=0;
				switch(i)
				{
					case B:
					t=9;
					action='B';
					break;
				
					case F:
					t=-9;
					action='F';
					break;
				
					case L:
					t=-1;
					action='L';
					break;
				
					case R:
					t=1;
					action='R';	
					break;
				
					case D:
					t=3;
					action='D';	
					break;
				
					case U:
					t=-3;
					action='U';
					break;
				}
				if (x+t<0||x+t>26||s.map[x+t] == -1)
				{
					continue;
				}			
					
				state tmp=s;		
				swap(tmp.map[x], tmp.map[x+t]);
				tmp.path+=(action);
				tmp.h=s.h+tryswap(x+t,x,tmp.map[x]);
				tmp.g++;
				open.push(tmp);		
			}
		}
	}
	return "ERROR";
}

int main()
{
	state source;
	string result;
	int a[27];
	FILE *fp;
	fp = fopen("input.txt", "r");
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)	
			fscanf(fp,"%d %d %d\n",&a[3*j+9*i],&a[1+3*j+9*i],&a[2+3*j+9*i]);
		fscanf(fp,"\n");
	}
	fclose(fp);

	fp = fopen("target.txt", "r");
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)	
			fscanf(fp,"%d %d %d\n",&target[3*j+9*i],&target[1+3*j+9*i],&target[2+3*j+9*i]);
		fscanf(fp,"\n");
	}
	fclose(fp);
	
	for(int i=0;i<27;i++)
	{
		if(target[i]==-1||a[i]==-1)
		{
			if(a[i]!=target[i])
			{
				printf("Input false\n");
				return -1;
			}
		}
	}
	
	source.setnum(a);
	end.setnum(target);
	source.h=-1;
 
 	int a0[27]={0};
	int b0[27]={0};
 	
	//数组内容下标互换(好烦 
	for(int i=0;i<27;i++)
	{
		if(a[i]!=-1)
		{
			a0[a[i]]=i;
		}
		if(target[i]!=-1)
		{
			b0[target[i]]=i;
		}
	}
	for(int i=1;i<27;i++)
	{
		source.h+=h2(a0[i],b0[i]);
	}
	
			
			
			
	//找到对应位置 
	for(int i=0;i<27;i++)
		if(end.map[i]!=-1)
			position[end.map[i]]=i;

	clock_t start,finish;
	start = clock();
	result = astar(source);
	finish = clock();

	double totaltime = (double)(finish - start) / CLOCKS_PER_SEC;

	fstream fout;
	fout.open("output_Ah2.txt", ios_base::out);
	fout.setf(ios::fixed); //fprintf乱码，我也很绝望 
	fout << totaltime << endl;
	fout << result << endl;
	fout.close();

	return 0;
}
