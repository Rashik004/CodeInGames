/*
9 9 2 1
1 7 4
8 7 3
1
2 5 V
*/


#include <bits/stdc++.h>
#define INF 1000000
#define mp make_pair
#define pb push_back
#define pii pair<int,int>
#define mem(ara, value) memset(ara, value, sizeof(ara))
#define MAXPATH 10000
using namespace std;
int w; // width of the board
int h; // height of the board
int myId; // id of my player (0 = 1st player, 1 = 2nd player, ...)
//int ultimateDirection;
int playersX[4], playersY[4], playersWallsLeft[4];
bool wallPlacement[40][40];
int dirY[]={-1, +0, +1, +0};
int dirX[]={+0, +1, +0, -1};
int cost[15][15];
int playerCount; // number of players (2 or 3)
int targetPlayer;
char orientationList[]="VH";
string directions[]={ "UP", "RIGHT", "DOWN", "LEFT"};
bool validPlayer[3];
struct WallDetails{
    bool top;
    bool left;
};

struct MoveDetails{
    int direnctionIndex;
    int cost;
};
struct Point{
    int x;
    int y;
};
struct Wall{
    Point point;
    char orientation;
    bool isInitialized;
   // Wall();

};


Wall initializeWall(int x, int y, int orientation)
{
    Wall result;
    result.point.x=x;
    result.point.y=y;
    result.orientation=orientation;
    result.isInitialized=true;
    return result;
}
struct Target{
    int targetX;
    int targetY;
    int ultimateDirection;
};
MoveDetails nextBestMoves[4];
WallDetails wallDetails[10][10];
bool placedWallStart[10][10][2];
bool reachable(int x, int y, int index)
{
    int nowX, nowY;
    nowX=x+dirX[index];
    nowY=y+dirY[index];
//    char s=' ';
    if(nowX<0 || nowX>=w || nowY<0 || nowY>=h)
    {
        return false;
    }
    bool wallExists=false;
    if(index==1)///check right side walls
    {
//        cerr<<nowX<<s<<y<<endl<<nowX<<s<<y+1<<endl<<endl;
        wallExists=wallDetails[nowX][nowY].left;
    }
    else if (index ==3)///check left side wall
    {
        wallExists=wallDetails[x][y].left;
    }
    else if (index==2)///check downstairs
    {
//        cerr<<x<<s<<nowY<<endl<<x+1<<s<<nowY<<endl<<endl;
        wallExists=wallDetails[nowX][nowY].top;
    }
    else /// check up
    {
        wallExists=wallDetails[x][y].top ;
    }
    return !wallExists;
}

Target GetTarget(int playerIndex)
{
    int targetX=-1,targetY=-1,ultimateDirection=-1;
    if(playerIndex==0)
    {
        targetX=w-1;
        ultimateDirection=1;
    }
    else if(playerIndex==1)
    {
        targetX=0;
        ultimateDirection=3;
    }
    else
    {
        targetY=h-1;
        ultimateDirection=2;
    }
    Target result;
    result.targetX=targetX;
    result.targetY=targetY;
    result.ultimateDirection=ultimateDirection;
    return result;
}
bool canPlaceWall(int x, int y, char orientation)
{
    int checkX, checkY,beforeX, beforeY, afterX, afterY;
    int orientationIndex= (orientation=='V');
    if(orientationIndex)
    {

        if(x==0 || y>7)
            return false;

        afterX=x+dirX[2];;/// Go DOWN
        afterY=y+dirY[2];;/// Go DOWN

        beforeX=x+dirX[0];/// Go UP
        beforeY=y+dirY[0];/// Go UP

        checkX=x+dirX[2]+dirX[3];/// Go DOWN+LEFT
        checkY=y+dirY[2]+dirY[3];/// Go DOWN+LEFT


    }
    else
    {
        if(y==0 || x>7)
            return false;

        afterX=x+dirX[1];/// Go RIGHT
        afterY=y+dirY[1];/// Go RIGHT

        beforeX=x+dirX[3];/// Go LEFT
        beforeY=y+dirY[3];/// Go LEFT

        checkX=x+dirX[0]+dirX[1];/// Go UP+RIGHT
        checkY=y+dirY[0]+dirY[1];/// Go UP+RIGHT
    }
   ///  If This wall intersects with another wall           If This wall is same as an existing wall                            If this wall falls within a part of another wall
    if(placedWallStart[checkX][checkY][!orientationIndex] || placedWallStart[x][y][orientationIndex] || placedWallStart[beforeX][beforeY][orientationIndex] || placedWallStart[afterX][afterY][orientationIndex])
    {
        return false;
    }
    return true;

}

void removeWall(int x, int y, int orientation)
{
    int nextDirection;
    placedWallStart[x][y][orientation=='V']=false;
    if(orientation=='H')
    {
        nextDirection=1;
    }
    else
    {
        nextDirection=2;
    }

    int nextX,nextY;
    nextX=x+dirX[nextDirection];
    nextY=y+dirY[nextDirection];
    if(orientation=='H')
    {
        wallDetails[x][y].top=false;
        wallDetails[nextX][nextY].top=false;
    }
    else
    {
        wallDetails[x][y].left=false;
        wallDetails[nextX][nextY].left=false;
    }

}

int bfs(int x, int y,Target target)
{
    queue< pii > CurrentPos;
    bool visited[15][15];
    mem(visited,false);
    pii now=mp(x,y);
    CurrentPos.push(now);
    visited[x][y]=true;
    cost[x][y]=1;
    int nowX,nowY,nextX,nextY;
    while(!CurrentPos.empty())
    {
        pii nowParent=CurrentPos.front();
        CurrentPos.pop();

        nowX=nowParent.first;
        nowY=nowParent.second;
        for(int i=0; i<4; i++)
        {
            nextX=nowX+dirX[i];
            nextY=nowY+dirY[i];
            if(!visited[nextX][nextY] && reachable(nowX,nowY, i))
            {
                visited[nextX][nextY]=true;
                cost[nextX][nextY]= cost[nowX][nowY]+1;
                CurrentPos.push(mp(nextX,nextY));
                if(nextX==target.targetX || nextY==target.targetY)
                {
                    return cost[nextX][nextY];
                }
            }
        }
    }
    cerr<<"Got no results!!!\n";
    return MAXPATH;
}

MoveDetails nextMove(int x, int y,Target target)
{
    int nowCost,nowX, nowY, best=INF, direction;

    MoveDetails moveDetails;
    for(int i=0; i<4; i++)
    {
        nowX=x+dirX[i];
        nowY=y+dirY[i];

        if(reachable(x,y,i))
        {
            if(nowX==target.targetX || nowY==target.targetY)
            {
                moveDetails.direnctionIndex=i;
                moveDetails.cost=1;
                return moveDetails;
            }
            nowCost=bfs(nowX,nowY,target);
            if(nowCost<best)
            {
                direction=i;
                best=nowCost;
            }
            else if(nowCost==best && i!=target.ultimateDirection)
            {
                direction=i;
            }
        }

    }
    moveDetails.direnctionIndex=direction;
    moveDetails.cost=best;
    if(best==MAXPATH)
    {
        cerr<<"2441139\n";
    }
    return moveDetails;
}

void PlaceWall(int x,int y,char wallOrientation)
{

    int index;
    if(wallOrientation=='H')
    {
        index=1;
        placedWallStart[x][y][0]=true;
        wallDetails[x][y].top=true;
        x+=dirX[index];
        y+=dirY[index];
        wallDetails[x][y].top=true;
    }
    else
    {
        index=2;
        placedWallStart[x][y][1]=true;
        wallDetails[x][y].left=true;
        x+=dirX[index];
        y+=dirY[index];
        wallDetails[x][y].left=true;
    }
}

//bool wallUnitTest()
//{
//    for(int i=0;i<9; i++)
//    {
//        for(int j=0;j<9; j++)
//        {
//            for(int k=0;k<2; k++)
//            {
//                if(canPlaceWall(i,j,orientationList[k]))
//                {
//                    PlaceWall(i,j,orientationList[k]);
//                    removeWall(i,j,orientationList[k]);
//                }
//            }
//        }
//    }
//
//    for(int i=0;i<9; i++)
//    {
//        for(int j=0;j<9; j++)
//        {
//            if(wallDetails[i][j].top || wallDetails[i][j].left || placedWallStart[i][j][0] || placedWallStart[i][j][1])
//                return false;
//        }
//    }
//    return true;
//}


Wall BestWall()
{
    MoveDetails currentMoves[4], bestMove[4];
    Wall bestWall;
    cerr<<"Entering Best";
    bestWall.isInitialized=false;
    bool attackedTarget=false;
    int bestResult=0;
    for(int i=0;i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
//            if(i==2 && j==5)
//            {
//                int as=34;
//                int s=3;
//
//            }
            for(int k=0;k<2; k++)
            {
//                if(!wallUnitTest())
//                {
////                    cout<<"jhamela!!! "<<i<<' '<<j<<' '<<k<<endl<<endl;
//                }

                if(!canPlaceWall(i,j,orientationList[k]))///check if we can place a wall
                {
                    continue;
                }
                cerr<<i<<' '<<j<< ' '<<k<<endl;
                PlaceWall(i,j,orientationList[k]);
                bool isBlocking=false;
                for(int player=0; player<playerCount; player++)///placed wall, now simulate each player's shortest path
                {
                    if(!validPlayer[player])
                        continue;
                    currentMoves[player]=nextMove(playersX[player],playersY[player],GetTarget(player));
                    if(currentMoves[player].cost==MAXPATH)
                        isBlocking=true;
                }
                removeWall(i,j,orientationList[k]);
                if(isBlocking)
                {
                    cerr<<"kaamer faad!!!";
                    continue;
                }
                if(currentMoves[myId].cost>nextBestMoves[myId].cost)///if this path becomes greater than my prev shortest path then abort
                {
                    continue;

                }
                for(int player=0; player<playerCount; player++)
                {
                    if(player==myId || currentMoves[player].cost<=nextBestMoves[player].cost)///if opponents shortest path has increase then move forward
                        continue;

                    if(player==targetPlayer )
                    {
                        if(!attackedTarget)
                        {
                            attackedTarget=true;
                            bestResult=currentMoves[player].cost-nextBestMoves[player].cost;
                            bestWall=initializeWall(i,j,orientationList[k]);
                        }
                        else if(currentMoves[player].cost-nextBestMoves[player].cost>bestResult)
                        {
                            bestResult=currentMoves[player].cost-nextBestMoves[player].cost;
                            bestWall=initializeWall(i,j,orientationList[k]);
                        }
                    }
                    else if(!attackedTarget)
                    {
                        if(currentMoves[player].cost>nextBestMoves[player].cost && bestResult>currentMoves[player].cost-nextBestMoves[player].cost)
                        {
                            if(nextBestMoves[player].cost==nextBestMoves[myId].cost && myId>player)
                            {
                                bestResult=currentMoves[player].cost-nextBestMoves[player].cost;
                                bestWall=initializeWall(i,j,orientationList[k]);
                            }
                            else if(nextBestMoves[player].cost<nextBestMoves[myId].cost)
                            {
                                bestResult=currentMoves[player].cost-nextBestMoves[player].cost;
                                bestWall=initializeWall(i,j,orientationList[k]);
                            }
                        }
                    }
                }

            }
        }
    }
    cerr<<"outta jungle!!!";
    return bestWall;
}

bool isPalinDrome(string A)
{
    cerr<<A<<endl;
    int s=A.size()-1;
    for(int i=0;i<=s/2; i++)
    {
        if(A[i]!=A[s-i])
            return false;
    }
    return true;
}

int isPalindrome(string A) {
    // Do not write main() function.
    // Do not read input, instead use the arguments to the function.
    // Do not print the output, instead return values as specified
    // Still have a doubt. Checkout www.interviewbit.com/pages/sample_codes/ for more details
    string mainString;
    for(int i=0; i<A.size(); i++)
    {
        if(A[i]>='A' && A[i]<='Z')
            mainString.push_back(A[i]);
        else if(A[i]>='a' && A[i]<='z')
        {
            mainString.push_back(A[i]-32);
        }
    }
    return isPalinDrome(mainString);

}








int main()
{

    string abc;
    getline(cin, abc);
    cout<<isPalindrome(abc)<<endl;
}
