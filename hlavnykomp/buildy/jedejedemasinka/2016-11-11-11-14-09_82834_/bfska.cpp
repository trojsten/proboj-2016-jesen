#include<bits/stdc++.h>

#define ll long long
#define ff first
#define ss second

#define pll pair<long long, long long>

using namespace std;

//int dx[]={-1, -1, 0, 1, 1, 1, 0, -1};
//int dy[]={0, 1, 1, 1, 0, -1, -1, -1};

int dx[]={-1, 0, 1, 0};
int dy[]={0, 1, 0, -1};

vector<vector<int> > distance(vector<vector<int> >&vstup)
//0-oddrbe ta
//1-neoddrbe ta
//2-domcek
{
    int a, b, c, na, nb;
    int r = vstup.size(), s=vstup[0].size();
    queue<int>Q;
    
    vector<vector<int> >dist(r, vector<int>(s, -1));
    
    for(int i=0;i<r;++i)
    {
        for(int j=0;j<s;++j)
        {
            if(vstup[i][j] == 2)
            {
                Q.push(i);
                Q.push(j);
                Q.push(0);
                dist[i][j]=0;
            }
        }
    }
    
    while(!Q.empty())
    {
        a=Q.front();Q.pop();
        b=Q.front();Q.pop();
        c=Q.front();Q.pop();
        
            for(int i=0;i<4;++i)
            {
                na = a+dx[i];
                nb = b+dy[i];
                if(na>=0 && nb>=0 && na<r && nb<s && vstup[na][nb] != 0 && dist[na][nb] == -1)
                {
                    Q.push(na);
                    Q.push(nb);
                    Q.push(c+1);
                    dist[na][nb]=c+1;
                }
            }
    }
    
    return dist;
}

void vytratbfs(vector<vector<double> >&vstup, double konst, int iter)
{
    queue<int>Q;
    double sum;
    int num, ni, nj, r = vstup.size(), s=vstup[0].size();
    vector<vector<double> >odpoved(r, vector<double>(s, 0));

    double konstPren = konst;
    vector<vector<int> > susedia(r,vector<int> (s,0));
    for(int f=0;f<iter;++f)
    {
        
        for(int i = 0;i<r;++i)
        {
            for(int j = 0;j<s;++j)
            {
                num = 0;
                sum = 0.0;
                for(int k = 0;k<4;++k)
                {
                    ni = i+dx[k];
                    nj = j+dy[k];
                    if(ni>=0 && nj>=0 && ni<r && nj<s)
                    {
                        sum += vstup[ni][nj];
                        ++num;
                    }
                }
                susedia[i][j] = num;
                odpoved[i][j] += (sum);
            }
        }
        
        for(int i = 0;i<r;++i)
        {
            for(int j = 0;j<s;++j)
            {
                vstup[i][j] = (vstup[i][j]+konst*odpoved[i][j])/(konst*susedia[i][j]+1);
                odpoved[i][j]=0;
            }
        }
    }
    
}
/*
int main()
{
    //ios_base::sync_with_stdio(false);
    ll r, s;
    cin >> r >> s;
    vector<vector<double> >matica(r, vector<double>(s, 0));
    for(int i=0;i<r;++i)for(int j=0;j<s;++j)cin>>matica[i][j];
    cout<<"odpoved1 : "<<endl;
    vytratbfs(matica, 0.01, 10);
    for(int i=0;i<r;++i)
    {
        cout<<endl;
        for(int j=0;j<s;++j)
        {
            printf("%lf ", matica[i][j]);
            //cout<<matica[i][j]<<" ";
        }
    }
    return 0;
}
*/