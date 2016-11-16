#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Don't let the machines win. You are humanity's last hope...
 **/
string lines[40];
 bool CheckBounds(int h, int w, int x, int y)
 {
    if(x<0 || x>=h || y<0 || y>=w)
        return false;
    return true;
}

void GoLeft(int h, int w, int x, int y)
{
    if(!CheckBounds(h,w,x,y))
    {
        cout<<"-1 -1 ";
        return;
    }
    if(lines[x][y]=='0')
    {
        cout<<y<< ' '<<x<<' ';
        return;
    }
    GoLeft(h,w,x,y+1);
}

void GoBottom(int h, int w, int x, int y)
{
     if(!CheckBounds(h,w,x,y))
    {
        cout<<"-1 -1";
        return;
    }
    if(lines[x][y]=='0')
    {
        cout<<y<< ' '<<x;
        return;
    }
    GoBottom(h,w,x+1,y);
}
int main()
{
    int width; // the number of cells on the X axis
    cin >> width; cin.ignore();
    int height; // the number of cells on the Y axis
    cin >> height; cin.ignore();

    for (int i = 0; i < height; i++) {

        getline(cin, lines[i]);
    }

    for(int i=0;i<height; i++)
    {
        for(int j=0;j<width; j++)
        {
            if(lines[i][j]!='0')
                continue;
            cout<<j<<' '<<i<<' ';
            GoLeft(height, width, i, j+1);
            GoLeft(height, width, i+1, j);
            cout<<endl;
        }
    }
    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;


    // Three coordinates: a node, its right neighbor, its bottom neighbor
    //cout << "0 0 1 0 0 1" << endl;
}
