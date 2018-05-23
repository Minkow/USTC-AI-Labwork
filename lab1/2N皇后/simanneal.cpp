#include<iostream>
#include<stdlib.h>
#include<cstdio> 
#include<ctime>
#include<cstring> 
#include<cmath>

using namespace std;

int N;
int h;//h值即可以互相伤害的Queen个数
int *queen;
int *queen0; 
int *diagonal1;
int *diagonal2;//对角线数组维护对角线上Queen个数用作简化各种计算 ,1为/ 2为\ 

void swap(int &a,int &b)
{
	if(&a!=&b)
	{
		a^=b;
		b^=a;
		a^=b;
	}
}

void init()
{
	h=0;
	for(int i=0;i<N;i++)
		queen[i]=i;
	for(int i=0;i<N;i++)
		swap(queen[i],queen[(rand()%N)]);
	//特定的初始情况，只有对角线不满足，之后的操作只需要交换两个皇后的列位置即可(而不是跑棋盘 
	memset(diagonal2,0,sizeof(int)*(2*N-1));
	memset(diagonal1,0,sizeof(int)*(2*N-1));
	for(int i=0;i<N;i++)
	{
		diagonal1[i+queen[i]]++;//  /方向 
		diagonal2[N-1+i-queen[i]]++;// \方向
	}
	for(int i=0;i<2*N-1;i++)
	{
		if(diagonal1[i]>1)
			h+=diagonal1[i]-1;
		if(diagonal2[i]>1)
			h+=diagonal2[i]-1;
	}
	//初始对角线上的h值(由于初始的特殊性没有行列互相伤害) 
}

int tryswap(int a,int b)
{
	int dec,inc;
	int a1,a2,b1,b2,c1,c2,d1,d2;

	a1=a+queen[a];
	a2=a-queen[a];
	b1=b+queen[b];
	b2=b-queen[b];
	c1=a+queen[b];
	c2=a-queen[b];
	d1=b+queen[a];
	d2=b-queen[a];

	if(a1==b1)
	{
		dec=(diagonal2[N-1+a2]>1?1:0)+
			(diagonal2[N-1+b2]>1?1:0)+
			(diagonal1[a1]>2?2:1);
		
		inc=(diagonal1[c1]>0?1:0)+
			(diagonal1[d1]>0?1:0)+
			(diagonal2[N-1+c2]>0?2:1);
	}

	else if(a2==b2)
	{

		dec=(diagonal1[a1]>1?1:0)+
			(diagonal1[b1]>1?1:0)+
			(diagonal2[N-1+a2]>2?2:1);
		
		inc=(diagonal2[N-1+c2]>0?1:0)+
			(diagonal2[N-1+d2]>0?1:0)+
			(diagonal1[c1]>0?2:1);
	}


	else
	{
		dec=(diagonal1[a1]>1?1:0)+
			(diagonal1[b1]>1?1:0)+
			(diagonal2[N-1+a2]>1?1:0)+
			(diagonal2[N-1+b2]>1?1:0);
		  
		inc=(diagonal1[c1]>0?1:0)+
			(diagonal1[d1]>0?1:0)+
			(diagonal2[N-1+c2]>0?1:0)+
			(diagonal2[N-1+d2]>0?1:0);
	}
	
	return inc-dec;//变化量 
}


int nextstep(int a,int b)
{	
	diagonal1[a+queen[a]]--;
	diagonal1[b+queen[b]]--;
	diagonal2[N-1+a-queen[a]]--;
	diagonal2[N-1+b-queen[b]]--;
	
	swap(queen[a], queen[b]);

	diagonal1[a+queen[a]]++;
	diagonal1[b+queen[b]]++;
	diagonal2[N-1+a-queen[a]]++;
	diagonal2[N-1+b-queen[b]]++;
} 


void hillclimbing()
{
	h=1;
	int x;
	int hilltop=0;
	double T=1;
	while(h>0) 
	{
		hilltop=0;
		init();
		while(!hilltop)
		{
			hilltop=1;
			for(int i=0;i<N;i++)
				for(int j=i;j<N;j++)
					if(diagonal1[i+queen[i]]>1 || diagonal1[j+queen[j]]>1 || diagonal2[i-queen[i]+N-1]>1 || diagonal2[j-queen[j]+N-1]>1)
					{
						x=tryswap(i,j);
						if(x<0)
						{
							nextstep(i,j);
							h+=x;
							hilltop=0;
							if(h==0) goto OK;
						}
						else if(((exp(-x/T))>((double)(rand()%10000)/10000))&&x!=0)
						{
							nextstep(i,j);
							h+=x;
							hilltop=0;
							if(h==0) goto OK;
						}
						T*=0.999;
					}
		}

OK:		if(h==0)
		{
			if(N%2==0)
			{
				for(int i=0;i<N;i++)
					queen0[i]=N-1-queen[i];//偶数：轴对称 
			}
			else if(N!=5)
			{
				for(int i=0;i<N;i++)
				{
					if(queen[i]==i)
					{
						h=1;
						break;
					}
					else
					{
						queen0[queen[i]]=i;//奇数:对角线对称 
					}
				}
			}
			else 
			{
				queen0[0]=1;
				queen0[1]=3;
				queen0[2]=0;
				queen0[3]=2;
				queen0[4]=4;
			}
		}
	}

}

int main()
{
	FILE *fp;
	fp=fopen("input.txt","r");
	fscanf(fp,"%d",&N);
	fclose(fp);
	
	fp=fopen("output_simulated_annealing.txt","w");
	int h1=0;

	clock_t start, finish;
 	double totaltime;

//	scanf("%d",&N);
	queen=(int *)calloc(N,sizeof(int));
	diagonal1=(int *)calloc(2*N-1,sizeof(int));
	diagonal2=(int *)calloc(2*N-1,sizeof(int));
	queen0=(int *)calloc(N,sizeof(int));
	start = clock();
	hillclimbing();
	
	finish = clock();
	totaltime = (double)(finish-start)/CLOCKS_PER_SEC;

//	for(int i=1;i<N;i++)
//	{
//		if(diagonal1[i]>1)
//			h1+=diagonal1[i]-1;
//		if(diagonal2[i]>1)
//			h1+=diagonal2[i]-1;
//	}
	
//	printf("------%d\n",h1);

	for(int i=0;i<N;i++)
	{
		fprintf(fp,"%d\n",queen[i]+1);
	}
	for(int i=0;i<N;i++)
	{
		fprintf(fp,"%d\n",queen0[i]+1);
	}
	fprintf(fp,"%lf\n",totaltime);
} 
