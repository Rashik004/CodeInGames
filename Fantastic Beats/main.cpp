///Rashik Hasnat;
///uva, a2oj:Rashik, codeforces, codechef, codemarshal:rashik004;
///git: https://github.com/Rashik004/Coded-algorithms
///LinkedIn:bd.linkedin.com/in/rashikhasnat
///Khulna University of Engineering & Technology (KUET), Khulna
///Bangladesh
#include <bits/stdc++.h>
#define mp make_pair
#define pb push_back
#define pii pair<int,int>
#define ll long long
#define ull unisgned long long
#define read freopen("input.txt","r",stdin)
#define write freopen("output.txt","w",stdout)
#define vi vector<int>
#define MAX 10000+5
#define all(v) v.begin(), v.end()
#define PI acos(-1.0)
#define mem(ara, value) memset(ara, value, sizeof(ara))
#define sf scanf
#define Tcase(t) for(int qq=1;qq<=t; qq++)
using namespace std;

/// ******************************** ///

bool isLinier(int a, int b, int m, int n, int x, int y)
{
    int t1,t2;
    t1=(n-b)*(x-m);
    t2=(y-n)*(m-a);
    if(t1==t2)
        return true;
    else
        return false;
}


struct line{//ax+by=c
    void values(double x, double y, double z){a=x,b=y,c=z;}
    double a,b,c;};

pair<double,double> sectionPoint(line l1, line l2)//ax+by=c
{
    double a1,b1,c1,a2,b2,c2,x,y,dx,dy,d;

    a1=l1.a,
    b1=l1.b;
    c1=l1.c;
    a2=l2.a;
    b2=l2.b;
    c2=l2.c;
    d=a1*b2-b1*a2;
    if(d==0)
        return mp(-1, -1);
    dx=c1*b2-c2*b1;
    dy=a1*c2-a2*c1;
    x=dx*1.0/d;
    y=dy*1.0/d;
    cout<<x<<endl<<y<<endl;
    return mp(x,y);
}

line equation(pair<double,double> point1, pair<double,double> point2)
{
    double x1,x2,y1,y2;
    x1=point1.first;
    y1=point1.second;
    x2=point2.first;
    y2=point2.second;
    double kx,ky;
    line ret;
    kx=x1-x2;
    ky=y1-y2;
    ret.a=ky;
    ret.b=kx;
    ret.c=x1*ky-y1*kx;
    if(ret.a<0)
    {
        ret.a*=-1;
        ret.b*=-1;
        ret.c*=-1;
    }
    cout<<ret.a<<"x+"<<ret.b<<"y="<<ret.c<<endl;
    return ret;
}



double const eps = 1e-9;
ll manhattan(ll x1, ll y1, ll x2, ll y2)
{
    return abs(x1-x2)+abs(y1-y2);
}




///************main code starts from below******************///
//int grid[17000][4000][5];
int myPlayer[2];
int oponent[2];
int snafle[20];
int positionDetails[15][6];
int entities;
int highSnaffle;
int const WIZARDRADIUS=400;
int const SNAFFLERADIUS=150;
bool isHolding[15];
bool isOnField[15];
int myPlayerCount=0, oponentPlayerCount=0, snafleCount=0;
pii targetGoalBar;
///myPlayer 0, oponnent 1, snifle 2;
int getEntityTypeInt(string entityType)
{
    if(entityType=="WIZARD")
        return 0;
    if(entityType=="OPPONENT_WIZARD")
        return 1;
    if(entityType=="SNAFFLE")
        return 2;
}
int getNearestSnaffle(int wizardIndex)
{
    int resultIndex,nowDist,playerX,playerY, targetX, targetY;
    playerX=positionDetails[wizardIndex][1];
    playerY=positionDetails[wizardIndex][2];
    int mn=1<<30;
    cerr<<"Nearest search for wizard "<<wizardIndex<<' '<<playerX<<' '<<playerY<<":\n";
    for(int i=4; i<=highSnaffle; i++)
    {
        cerr<<"GOT HERE "<<i<<endl;
        if(!isOnField[i])
            continue;
        cerr<<"passed it!! "<<i<<endl;
        targetX=positionDetails[i][1];
        targetY=positionDetails[i][2];
        nowDist=manhattan(targetX,targetY,playerX, playerY);
        cerr<<'\t'<<i<<" "<<targetX<<' '<<targetY<<' '<<nowDist<<endl;
        if(nowDist<mn)
        {
            mn=nowDist;
            resultIndex=i;
        }
    }
    cerr<<endl<<endl;
    return resultIndex;
}

pii getTargetGoalBar(int playerIndex)
{
    if(playerIndex==1)
    {
        return mp(0,3750);
    }
    else
        return mp(16000,3750);
}

//void throwSniffle(int playerIndex, int sniffleIndex)
//{
//
//}

int main()
{
    int myTeamId; // if 0 you need to score on the right of the map, if 1 you need to score on the left
    cin >> myTeamId; cin.ignore();
    targetGoalBar=getTargetGoalBar(myTeamId);
    // game loop
    while (1) {
         // number of entities still in game
//        mem(grid, -1);
        mem(isHolding,false);
        mem(isOnField, false);
        highSnaffle=0;
        cin >> entities; cin.ignore();
        myPlayerCount=0, oponentPlayerCount=0, snafleCount=0;
        for (int i = 0; i < entities; i++) {
            int entityId; // entity identifier
            string entityType; // "WIZARD", "OPPONENT_WIZARD" or "SNAFFLE" (or "BLUDGER" after first league)
            int x; // position
            int y; // position
            int vx; // velocity
            int vy; // velocity
            int state; // 1 if the wizard is holding a Snaffle, 0 otherwise
            int entityTypeInt;
            cin >> entityId >> entityType ;
            entityTypeInt=getEntityTypeInt(entityType);
            positionDetails[entityId][0]=entityTypeInt;
            for(int j=1; j<=5; j++)
            {
                cin>> positionDetails[entityId][j];
            }
            isOnField[entityId]=true;
            isHolding[entityId]=positionDetails[entityId][5];
            if(entityTypeInt==2)
            {
                snafleCount++;
                highSnaffle=max(highSnaffle,entityId);
            }
        }

        int startIndex,nowIndex;
        if(myTeamId==1)
            startIndex=2;
        else
            startIndex=0;
        for (int i = 0; i < 2; i++) {

            // Write an action using cout. DON'T FORGET THE "<< endl"
            // To debug: cerr << "Debug messages..." << endl;


            // Edit this line to indicate the action for each wizard (0 ≤ thrust ≤ 150, 0 ≤ power ≤ 500)
            // i.e.: "MOVE x y thrust" or "THROW x y power"
            nowIndex=i+startIndex;
            for(int i=0;i <=10; i++)
            {
                cerr<<i<<' '<<isHolding[i]<<endl;
            }
            if(isHolding[nowIndex])
            {
                cout<<"THROW "<<targetGoalBar.first<<" "<<targetGoalBar.second<<" 500\n";
            }
            else
            {
                int targetedSnaffle=getNearestSnaffle(nowIndex);
                if(snafleCount!=1)
                {
                    isOnField[targetedSnaffle]=false;
                }
               // cerr<<targetedSnaffle<<endl;
                cout << "MOVE " << positionDetails[targetedSnaffle][1]<<" "<<positionDetails[targetedSnaffle][2]<<" 150\n";
            }

        }
    }
}






