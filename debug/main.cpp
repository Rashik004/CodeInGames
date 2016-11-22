/*
9 9 2 1
1 6 4
8 0 3
1
2 4 V
*/


#include <bits/stdc++.h>
#define INF 1000000
#define mp make_pair
#define pb push_back
#define pii pair<int,int>
#define mem(ara, value) memset(ara, value, sizeof(ara))
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
    int checkX, checkY;

    if(orientation=='V')
    {

        if(x==0 || y>7)
            return false;
                    if(x==2 && y==5)
            {
                int as=34;
                int s=3;

            }
        checkX=x+dirX[2]+dirX[3];/// Go DOWN+LEFT
        checkY=y+dirY[2]+dirY[3];/// Go DOWN+LEFT


    }
    else
    {
        if(y==0 || x>7)
            return false;
        checkX=x+dirX[0]+dirX[1];/// Go UP+RIGHT
        checkY=y+dirY[0]+dirY[1];/// Go UP+RIGHT
    }
    if(placedWallStart[checkX][checkY][orientation=='V'])
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
}

MoveDetails nextMove(int x, int y,Target target)
{
    int nowCost,nowX, nowY, best=INF, direction;

    MoveDetails moveDetails;
    for(int i=0; i<4; i++)
    {
        nowX=x+dirX[i];
        nowY=y+dirY[i];
        //cerr<<"Entering the jungle";
        if(reachable(x,y,i))
        {
            if(nowX==target.targetX || nowY==target.targetY)
            {
                moveDetails.direnctionIndex=i;
                moveDetails.cost=1;
                return moveDetails;
            }
            nowCost=bfs(nowX,nowY,target);
           // cerr<<nowCost<<endl<<endl;
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

bool wallUnitTest()
{
    for(int i=0;i<9; i++)
    {
        for(int j=0;j<9; j++)
        {
            for(int k=0;k<2; k++)
            {
                if(canPlaceWall(i,j,orientationList[k]))
                {
                    PlaceWall(i,j,orientationList[k]);
                    removeWall(i,j,orientationList[k]);
                }
            }
        }
    }

    for(int i=0;i<9; i++)
    {
        for(int j=0;j<9; j++)
        {
            if(wallDetails[i][j].top || wallDetails[i][j].left || placedWallStart[i][j][0] || placedWallStart[i][j][1])
                return false;
        }
    }
    return true;
}


Wall BestWall()
{
    MoveDetails currentMoves[4], bestMove[4];
    Wall bestWall;
//    for(int i=0;i<9;i++)
//    {
//        for(int j=0;j<9;j++)
//        {
//            for(int k=0;k<2; k++)
//            {
//                if(placedWallStart[i][j][k])
//                {
//                    cout<<' '<<i<<' '<<j<<' '<<k<<endl;
//                }
//            }
//        }
//    }
    bestWall.isInitialized=false;
    bool attackedTarget=false;
    int bestResult=0;
    for(int i=0;i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            if(i==2 && j==5)
            {
                int as=34;
                int s=3;

            }
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
                PlaceWall(i,j,orientationList[k]);
                for(int player=0; player<playerCount; player++)///placed wall, now simulate each player's shortest path
                {
                    currentMoves[player]=nextMove(playersX[player],playersY[player],GetTarget(player));
                }
                removeWall(i,j,orientationList[k]);
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
    return bestWall;
}








int main()
{

//    int targetX=-1,targetY=-1;
//    cout<<wallUnitTest()<<endl<<endl;
    cin >> w >> h >> playerCount >> myId; cin.ignore();
//    if(myId==0)
//    {
//        targetX=w-1;
//        ultimateDirection=1;
//    }
//    else if(myId==1)
//    {
//        targetX=0;
//        ultimateDirection=3;
//    }
//    else
//    {
//        targetY=h-1;
//        ultimateDirection=2;
//    }
    while (1)
    {
        mem(cost, -1);
        mem(placedWallStart,false);
        for (int i = 0; i < playerCount; i++)
        {
            int x; // x-coordinate of the player
            int y; // y-coordinate of the player
            int wallsLeft; // number of walls available for the player
            cin >> x >> y >> wallsLeft; cin.ignore();
            playersX[i]=x;
            playersY[i]=y;
            playersWallsLeft[i]=wallsLeft;
        }
        int wallCount; // number of walls on the board
        cin >> wallCount; cin.ignore();
        for (int i = 0; i < wallCount; i++)
        {
            int wallX; // x-coordinate of the wall
            int wallY; // y-coordinate of the wall
            char wallOrientation; // wall orientation ('H' or 'V')
            cin >> wallX >> wallY >> wallOrientation; cin.ignore();
            PlaceWall(wallX, wallY, wallOrientation);
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;



        int myCost,myNextDirection,nextShortestDistance;

        for(int i=0;i<playerCount; i++)
        {
            Target target=GetTarget(i);
            nextBestMoves[i]=nextMove(playersX[i],playersY[i], target);
        }

        myCost=nextBestMoves[myId].cost;
        myNextDirection=nextBestMoves[myId].direnctionIndex;

        bool isSomeoneAhead=false;

        for(int i=0;i<playerCount; i++)
        {
            if(i==myId)
                continue;
            int currentPlayerCost;

            currentPlayerCost=nextBestMoves[i].cost;

            if(currentPlayerCost==myCost && myId>i)
            {
                targetPlayer=i;
                isSomeoneAhead=true;
                break;
            }
            else if(currentPlayerCost<myCost && !isSomeoneAhead)
            {
                isSomeoneAhead=true;
                targetPlayer=i;
            }

            else if(currentPlayerCost<myCost && currentPlayerCost>nextBestMoves[targetPlayer].cost)
            {
                isSomeoneAhead=true;
                targetPlayer=i;
            }
        }
//        cout<<directions[myNextDirection]<<endl;
        if(!isSomeoneAhead)
        {
            cout<<directions[myNextDirection]<<endl;
            continue;
        }
//        else
//        {
//
//        }
        bool gotAresult;
        if(isSomeoneAhead)
        {
            Wall wall=BestWall();
            if(wall.isInitialized)
            {
                cout<<wall.point.x<<' '<<wall.point.y<<' '<<wall.orientation<<endl;
                gotAresult=true;
            }
        }
        else if(gotAresult)
        {
            cout<<directions[myNextDirection]<<endl;
        }
    }
}
