//
//  main.c
//  lfgame
//
//  Created by 鄭博元 on 2022/10/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int life_game[10][10];
    int temp[10][10];
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            life_game[i][j]=rand()%2;//取0~1的隨機數，1為活著，0為死亡
        }
    }
    int year=1;
    int end;
    printf("請輸入要模擬的代數");
    scanf("%d",&end);
    printf("\n");
    
    while(year<=end){
        for(int i=1;i<9;i++){//邊角先不處理
            for(int j=1;j<9;j++){
                int count=0;
                if(life_game[i-1][j-1]==1)
                    count+=1;
                if (life_game[i-1][j]==1)
                    count+=1;
                if (life_game[i-1][j+1]==1)
                    count+=1;
                if (life_game[i][j-1]==1)
                    count+=1;
                if (life_game[i][j+1]==1)
                    count+=1;
                if (life_game[i+1][j-1]==1)
                    count+=1;
                if (life_game[i+1][j]==1)
                    count+=1;
                if (life_game[i+1][j+1]==1)
                    count+=1;//檢查上下左右八個地方的存活數量
                if (life_game[i][j]==0){//原細胞死亡情況
                    if (count==3){
                        temp[i][j]=1;//復活情況
                    }
                    else{
                        temp[i][j]=0;//保持死亡
                    }
                }
                else{
                    if (count<2 || count>3){
                        temp[i][j]=0;//死亡情況
                    }
                    else{
                        temp[i][j]=1;//保持活著
                    }
                }
            }
        }
        //接下來處理四個角，依序為左上，右上，左下，右下
        int count=0;
        if (life_game[0][1]==1)
            count+=1;
        if (life_game[1][0]==1)
            count+=1;
        if (life_game[1][1]==1)
            count+=1;
        if (life_game[0][0]==0){//原細胞死亡情況
            if (count==3){
                temp[0][0]=1;//復活情況
            }
            else{
                temp[0][0]=0;//保持死亡
            }
        }
        else{
            if (count<2 || count>3){
                temp[0][0]=0;//死亡情況
            }
            else{
                temp[0][0]=1;//保持活著
            }
        }
        count=0;
        if (life_game[0][8]==1)
            count+=1;
        if (life_game[1][8]==1)
            count+=1;
        if (life_game[1][9]==1)
            count+=1;
        if (life_game[0][9]==0){//原細胞死亡情況
            if (count==3){
                temp[0][9]=1;//復活情況
            }
            else{
                temp[0][9]=0;//保持死亡
            }
        }
        else{
            if (count<2 || count>3){
                temp[0][9]=0;//死亡情況
            }
            else{
                temp[0][9]=1;//保持活著
            }
        }
        count=0;
        if (life_game[8][0]==1)
            count+=1;
        if (life_game[8][1]==1)
            count+=1;
        if (life_game[9][1]==1)
            count+=1;
        if (life_game[9][0]==0){//原細胞死亡情況
            if (count==3){
                temp[9][0]=1;//復活情況
            }
            else{
                temp[9][0]=0;//保持死亡
            }
        }
        else{
            if (count<2 || count>3){
                temp[9][0]=0;//死亡情況
            }
            else{
                temp[9][0]=1;//保持活著
            }
        }
        count=0;
        if (life_game[9][8]==1)
            count+=1;
        if (life_game[8][8]==1)
            count+=1;
        if (life_game[8][9]==1)
            count+=1;
        if (life_game[9][9]==0){//原細胞死亡情況
            if (count==3){
                temp[9][9]=1;//復活情況
            }
            else{
                temp[9][9]=0;//保持死亡
            }
        }
        else{
            if (count<2 || count>3){
                temp[9][9]=0;//死亡情況
            }
            else{
                temp[9][9]=1;//保持活著
            }
        }
        //接下來處理四條邊，依序為上，左，右，下
        for(int i=1;i<9;i++){
            count=0;
            if (life_game[1][i-1]==1)
                count+=1;
            if (life_game[1][i]==1)
                count+=1;
            if (life_game[1][i+1]==1)
                count+=1;
            if (life_game[0][i]==0){//原細胞死亡情況
                if (count==3){
                    temp[0][i]=1;//復活情況
                }
                else{
                    temp[0][i]=0;//保持死亡
                }
            }
            else{
                if (count<2 || count>3){
                    temp[0][i]=0;//死亡情況
                }
                else{
                    temp[0][i]=1;//保持活著
                }
            }
        }
        for(int i=1;i<9;i++){
            count=0;
            if (life_game[i-1][1]==1)
                count+=1;
            if (life_game[i][1]==1)
                count+=1;
            if (life_game[i+1][1]==1)
                count+=1;
            if (life_game[i][0]==0){//原細胞死亡情況
                if (count==3){
                    temp[i][0]=1;//復活情況
                }
                else{
                    temp[i][0]=0;//保持死亡
                }
            }
            else{
                if (count<2 || count>3){
                    temp[i][0]=0;//死亡情況
                }
                else{
                    temp[i][0]=1;//保持活著
                }
            }
        }
        for(int i=1;i<9;i++){
            count=0;
            if (life_game[i-1][8]==1)
                count+=1;
            if (life_game[i][8]==1)
                count+=1;
            if (life_game[i+1][8]==1)
                count+=1;
            if (life_game[i][9]==0){//原細胞死亡情況
                if (count==3){
                    temp[i][9]=1;//復活情況
                }
                else{
                    temp[i][9]=0;//保持死亡
                }
            }
            else{
                if (count<2 || count>3){
                    temp[i][9]=0;//死亡情況
                }
                else{
                    temp[i][9]=1;//保持活著
                }
            }
        }
        for(int i=1;i<9;i++){
            count=0;
            if (life_game[9][i-1]==1)
                count+=1;
            if (life_game[9][i]==1)
                count+=1;
            if (life_game[9][i+1]==1)
                count+=1;
            if (life_game[9][i]==0){//原細胞死亡情況
                if (count==3){
                    temp[9][i]=1;//復活情況
                }
                else{
                    temp[9][i]=0;//保持死亡
                }
            }
            else{
                if (count<2 || count>3){
                    temp[9][i]=0;//死亡情況
                }
                else{
                    temp[9][i]=1;//保持活著
                }
            }
        }
        //都處理完了，把存到temp裡頭的新狀態丟回life_game，然後再開始下一代
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                life_game[i][j]=temp[i][j];
                printf("%d ",life_game[i][j]);
            }
            printf("\n");
        }
        printf("==第%d代的模擬結束==\n\n",year);
        year+=1;
    }
}