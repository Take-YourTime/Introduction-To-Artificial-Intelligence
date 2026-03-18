#include<iostream>
#include<windows.h>
#include<string>
using namespace std;

int const n = 10;
int newnum[n+2][n+2] = {
                        0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,1,0,0,0,0,0,0,0,
                        0,0,1,0,1,0,0,0,0,0,0,0,
                        0,0,0,1,1,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0
                    };
int oldnum[n+2][n+2];
string map[n+2][n+2];

void gotoxy(int x,int y)
{   
   COORD postion;
   postion.X = x;
   postion.Y = y; 
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),postion);
}

int main() {
    while (true) /*for (int m = 0; m <= 10; m++) */{
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (newnum[i][j] == 1)
                    map[i][j] = "■";
                if (newnum[i][j] == 0)
                    map[i][j] = "□";
                cout << map[i][j] << '\t';
            }
            cout << endl;
        }
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                oldnum[i][j] = newnum[i][j];
            }
        }
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                int surrnum = oldnum[i-1][j-1]+oldnum[i-1][j]+oldnum[i-1][j+1]+oldnum[i][j-1]+oldnum[i][j+1]+oldnum[i+1][j-1]+oldnum[i+1][j]+oldnum[i+1][j+1];
                if (oldnum[i][j] == 1 && surrnum != 2 && surrnum != 3) 
                        newnum[i][j] = 0;
                if (oldnum[i][j] == 0 && surrnum == 3) 
                     newnum[i][j] = 1;
            }
        }
        // cout << endl << endl;
        Sleep(300);
        int temp = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (newnum[i][j] == oldnum[i][j])
                    temp += 1;
            }
        }
        gotoxy(0,0);
        if (temp == n*n)
            break;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cout << map[i][j] << '\t';
        }
        cout << endl;
    }
    
    return 0;
}