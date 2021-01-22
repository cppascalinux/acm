#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cassert>
#define ll long long
using namespace std;
int n;
char s[100009];
int pre[100009],lst[29];
int sa[100009],rnk[100009],h[100009],t1[100009],t2[100009],c[100009];
int f[29][100009],lg[100009];
int nxt[100009][29];
int sp[100009];
void getsa()
{
	int *x=t1,*y=t2,m=n;
	for(int i=1;i<=n;i++)
		c[x[i]=pre[i]]++;
	for(int i=1;i<=m;i++)
		c[i]+=c[i-1];
	for(int i=1;i<=n;i++)
		sa[c[x[i]]--]=i;
	for(int k=1;k<=n;k<<=1)
	{
		int p=0;
		for(int i=n-k+1;i<=n;i++)
			y[++p]=i;
		for(int i=1;i<=n;i++)
			if(sa[i]>k)
				y[++p]=sa[i]-k;
		assert(p==n);
		memset(c,0,(m+1)*4);
		for(int i=1;i<=n;i++)
			c[x[i]]++;
		for(int i=1;i<=m;i++)
			c[i]+=c[i-1];
		for(int i=n;i>=1;i--)
			sa[c[x[y[i]]]--]=y[i];
		p=0;
		swap(x,y);
		for(int i=1;i<=n;i++)
			x[sa[i]]=y[sa[i]]==y[sa[i-1]]&&y[sa[i]+k]==y[sa[i-1]+k]?p:++p;
		if((m=p)==n)
			break;
	}
}
void geth()
{
	for(int i=1;i<=n;i++)
		rnk[sa[i]]=i;
	int p=0;
	for(int i=1;i<=n;i++)
	{
		if(p)
			p--;
		int j=sa[rnk[i]-1];
		while(pre[i+p]==pre[j+p])
			p++;
		h[rnk[i]]=p;
	}
}
void initlcs()
{
	for(int i=1;i<=n;i++)
		f[0][i]=h[i];
	for(int k=1;(1<<k)<=n;k++)
		for(int i=1;i+(1<<k)-1<=n;i++)
			f[k][i]=min(f[k-1][i],f[k-1][i+(1<<(k-1))]);
	for(int i=2;i<=n;i++)
		lg[i]=lg[i>>1]+1;
}
int getlcs(int la,int ra,int lb,int rb)
{
	// printf("la:%d ra:%d lb:%d rb:%d\n",la,ra,lb,rb);
	int ans=min(ra-la+1,rb-lb+1);
	if(ans==0||la==lb)
		return ans;
	la=rnk[la],lb=rnk[lb];
	if(la>lb)
		swap(la,lb);
	int d=lg[lb-la];
	int l=min(f[d][la+1],f[d][lb-(1<<d)+1]);
	return min(ans,l);
}
int cmpstr(int la,int ra,int lb,int rb)
{
	int lcs=getlcs(la,ra,lb,rb);
	int lena=ra-la+1,lenb=rb-lb+1;
	if(lcs==lena&&lcs==lenb)
		return 0;
	if(lcs==lena)
		return 1;
	if(lcs==lenb)
		return -1;
	return pre[la+lcs]<pre[lb+lcs]?1:-1;
}
int cmp(int a,int b)
{
	if(a==b)
		return 0;
	int ans=0;
	if(b<a)
		swap(a,b),ans^=1;
	int curp=0,maxl=n-b+1;
	for(int i=1;i<=26;i++)
	{
		int na=nxt[a][i],nb=nxt[b][i];
		int mn=min(na,nb);
		int cp=cmpstr(a+curp,a+mn-1,b+curp,b+mn-1);
		// printf("i:%d curp:%d maxl:%d na:%d nb:%d cp:%d\n",i,curp,maxl,na,nb,cp);
		if(cp)
			return (cp==1)^ans;
		if(na!=nb)
			return (na<nb)^ans;
		if((curp=na+1)>=maxl)
			return 0^ans;
	}
	// printf("qwq");
	int cp=cmpstr(a+curp,a+maxl-1,b+curp,b+maxl-1);
	if(cp)
		return (cp==1)^ans;
	return 0^ans;
}
int cal(int a,int b)
{
	if(a==b)
		return n-a+1;
	if(b<a)
		swap(a,b);
	int curp=0,maxl=n-b+1;
	for(int i=1;i<=26;i++)
	{
		int na=nxt[a][i],nb=nxt[b][i];
		int mn=min(na,nb);
		int lcs=getlcs(a+curp,a+mn-1,b+curp,b+mn-1);
		if(lcs<mn-curp)
			return curp+lcs;
		if(na!=nb)
			return min(na,nb);
		if((curp=na+1)>=maxl)
			return maxl;
	}
	int lcs=getlcs(a+curp,a+maxl-1,b+curp,b+maxl-1);
	return curp+lcs;
}
int main()
{
	scanf("%d%s",&n,s+1);
	for(int i=1;i<=n;i++)
		s[i]-='a'-1;
	for(int i=1;i<=n;i++)
		pre[i]=i-lst[s[i]],lst[s[i]]=i;
	for(int i=1;i<=26;i++)
		lst[i]=n+1;
	for(int i=n;i>=1;i--)
	{
		lst[s[i]]=i;
		for(int j=1;j<=26;j++)
			nxt[i][j]=lst[j]-i;
		sort(nxt[i]+1,nxt[i]+27);
	}
	// for(int i=1;i<=n;i++)
	// 	for(int j=1;j<=3;j++)
	// 		printf("i:%d j:%d nxt:%d\n",i,j,nxt[i][j]);
	getsa();
	geth();
	// for(int i=1;i<=n;i++)
	// 	printf("i:%d pre:%d sa:%d rnk:%d h:%d\n",i,pre[i],sa[i],rnk[i],h[i]);
	initlcs();
	for(int i=1;i<=n;i++)
		sp[i]=i;
	sort(sp+1,sp+n+1,cmp);
	// printf("%d\n",cmp(1,2));
	// printf("%d\n",getlcs(2,5,3,5));
	// for(int i=1;i<=n;i++)
	// 	printf("i:%d sp:%d\n",i,sp[i]);
	ll ans=n-sp[1]+1;
	for(int i=2;i<=n;i++)
		ans+=(n-sp[i]+1)-cal(sp[i],sp[i-1]);
	printf("%lld",ans);
	return 0;
}