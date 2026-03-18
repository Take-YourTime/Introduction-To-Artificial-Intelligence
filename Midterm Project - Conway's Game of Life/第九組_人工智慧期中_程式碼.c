#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>
#define true 1
#define false 0
#define x_side 80 //x軸長度
#define y_side 40 //y軸長度
#define sec 10 //延遲秒數(單位：毫秒)

void gotoxy(int x, int y){//刷新頁面
	COORD pos = {x,y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}


void Show_Cursor(int visible) { //隱藏光標
    CONSOLE_CURSOR_INFO cursor_info = {20, visible};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}


int main(){
	int x=0,y=0;//座標，用來設定初始存活細胞的位置
	int sum;//某細胞周圍存活細胞的數量
	int count=1;//年代
	int mode=0;//模式
	char life_a[y_side+1+1][x_side+1+1],life_b[y_side+1+1][x_side+1+1];//(x_side+1)x(y_side+1)
	/* 
	+2用於設置矩陣外框，外框不進行運算，細胞永遠為死亡狀態。
	如沒外框，需分別處理三個部分：四角、四邊、中間
	*/
		
    for( int i = 0; i <= y_side+1; i++ ){//歸零
        for( int j = 0; j <= x_side+1; j++ ){
            life_a[i][j]='.';
            life_b[i][j]='.';
        }
    }
    
    start: //輸錯時，重新執行處
    
    printf("請選擇模式！\n\n");
    printf("1：【自訂模式】\n2：【高斯帕滑翔翼機槍】\n3：【SOS】\n4：【振盪器（oscillators）】\n5：【隨機模式】\n");
    printf("模式：");
    scanf("%d",&mode);
    
    if(mode==1){//自訂模式
    	printf("\n[自訂模式]\n");
    	printf("請輸入座標，範圍 X軸：1~%d、Y軸：1~%d\n",x_side,y_side);
		printf("若已輸入完畢，請輸入(-1,-1)\n");
    	fflush(stdin);
        while(scanf("%d%d",&x,&y)){//初始位置
            if(x==-1)//輸入-1 → 跳出
                break;
            //將輸入數字化為確切座標，方便閱讀
            life_a[y][x]='#';
        }
    }
    else if(mode==2){//高帕斯滑翔翼機槍
        life_a[1][26]='#';
        life_a[2][23]='#';life_a[2][24]='#';life_a[2][25]='#';life_a[2][26]='#';life_a[2][31]='#';
        life_a[3][14]='#';life_a[3][22]='#';life_a[3][23]='#';life_a[3][24]='#';life_a[3][25]='#';life_a[3][31]='#';
        life_a[4][13]='#';life_a[4][15]='#';life_a[4][22]='#';life_a[4][25]='#';life_a[4][35]='#';life_a[4][36]='#';
        life_a[5][12]='#';life_a[5][16]='#';life_a[5][17]='#';life_a[5][22]='#';life_a[5][23]='#';life_a[5][24]='#';life_a[5][25]='#';life_a[5][35]='#';life_a[5][36]='#';
        life_a[6][1]='#';life_a[6][2]='#';life_a[6][12]='#';life_a[6][16]='#';life_a[6][17]='#';life_a[6][23]='#';life_a[6][24]='#';life_a[6][25]='#';life_a[6][26]='#';
        life_a[7][1]='#';life_a[7][2]='#';life_a[7][12]='#';life_a[7][16]='#';life_a[7][17]='#';life_a[7][26]='#';
        life_a[8][13]='#';life_a[8][15]='#';
        life_a[9][14]='#';
    }
    else if(mode==3){//SOS
        life_a[1][4]='#';life_a[1][18]='#';
        life_a[2][2]='#';life_a[2][3]='#';life_a[2][4]='#';life_a[2][16]='#';life_a[2][17]='#';life_a[2][18]='#';
        life_a[3][1]='#';life_a[3][10]='#';life_a[3][11]='#';life_a[3][15]='#';
        life_a[4][2]='#';life_a[4][3]='#';life_a[4][4]='#';life_a[4][9]='#';life_a[4][12]='#';life_a[4][16]='#';life_a[4][17]='#';life_a[4][18]='#';
        life_a[5][5]='#';life_a[5][9]='#';life_a[5][12]='#';life_a[5][19]='#';
        life_a[6][2]='#';life_a[6][3]='#';life_a[6][4]='#';life_a[6][10]='#';life_a[6][11]='#';life_a[6][16]='#';life_a[6][17]='#';life_a[6][18]='#';
        life_a[7][2]='#';life_a[7][16]='#';
    }
    else if(mode==4){//振盪器（oscillators）
    	/* blinker */
    	life_a[3][3]='#';life_a[3][4]='#';life_a[3][5]='#';
    	/*------------------------------*/
    	/* pulsar */
    	life_a[8][8]='#';life_a[8][9]='#';life_a[8][10]='#';life_a[8][14]='#';life_a[8][15]='#';life_a[8][16]='#';
    	
		life_a[10][6]='#';life_a[10][11]='#';life_a[10][13]='#';life_a[10][18]='#';
		life_a[11][6]='#';life_a[11][11]='#';life_a[11][13]='#';life_a[11][18]='#';
		life_a[12][6]='#';life_a[12][11]='#';life_a[12][13]='#';life_a[12][18]='#';
		
		life_a[13][8]='#';life_a[13][9]='#';life_a[13][10]='#';life_a[13][14]='#';life_a[13][15]='#';life_a[13][16]='#';
		life_a[15][8]='#';life_a[15][9]='#';life_a[15][10]='#';life_a[15][14]='#';life_a[15][15]='#';life_a[15][16]='#';
		
		life_a[16][6]='#';life_a[16][11]='#';life_a[16][13]='#';life_a[16][18]='#';
		life_a[17][6]='#';life_a[17][11]='#';life_a[17][13]='#';life_a[17][18]='#';
		life_a[18][6]='#';life_a[18][11]='#';life_a[18][13]='#';life_a[18][18]='#';
		
		life_a[20][8]='#';life_a[20][9]='#';life_a[20][10]='#';life_a[20][14]='#';life_a[20][15]='#';life_a[20][16]='#';
	}
	else if(mode==5){ //隨機輸入
		int det=0;
		srand(time(NULL));
		for(int i=1;i<=y_side;i++){
			for(int j=1;j<=x_side;j++){
				det=rand()%2;
				if(det==true)
					life_a[i][j]='#';
			}
		}
	}
    else{
    	printf("無效的輸入\n\n");
    	goto start;//輸入錯誤時重新輸入 (line 34)
	}
	
	Show_Cursor(false);
	/*------------------------------*/
	gotoxy(0,0);
	printf(">>====%d====<<\n",count);//印出初始位置
	for(int i=1;i<=y_side;i++){
		for(int j=1;j<=x_side;j++){
			printf("%c",life_a[i][j]);
		}
		printf("\n");
	}
	Sleep(sec); //使程式暫停sec毫秒
	/*------------------------------*/
	/* (x,y) = (j,i)、(n,k) */
	
	while(true){ //矩陣A → 矩陣B
		for(int i=1;i<=y_side;i++){ //(j,i)為矩陣上的一點
			for(int j=1;j<=x_side;j++){
				sum=0;
				for(int k=i-1;k<=i+1;k++) //(n,k)為(j,i)周圍的點
					for(int n=j-1;n<=j+1;n++)
						if(life_a[k][n]=='#')
							sum++;
							
				if(life_a[i][j]=='#'){
					sum=sum-1; //如果(j,i)原本為「存活」，sum會多計算一次，要將其扣掉
					if( (sum<=1) || (sum<9&&sum>3) )
						life_b[i][j]='.';
					else
						life_b[i][j]='#';
				}
				else if(sum==3)
					life_b[i][j]='#';
				else
					life_b[i][j]='.';
			}
		}
		/*------------------------------*/
		gotoxy(0,0); //印出矩陣B
		count++;
		printf(">>====%d====<<\n",count);
		for(int i=1;i<=y_side;i++){
			for(int j=1;j<=x_side;j++){
				printf("%c",life_b[i][j]);
			}
			printf("\n");
		}
		Sleep(sec);
		
		/*------------------------------------------------------------*/
		
		/*------------------------------------------------------------*/
		
		for(int i=1;i<=y_side;i++){ //矩陣B → 矩陣A
			for(int j=1;j<=x_side;j++){
				sum=0;
				for(int k=i-1;k<=i+1;k++)
					for(int n=j-1;n<=j+1;n++)
						if(life_b[k][n]=='#')
							sum++;
							
				if(life_b[i][j]=='#'){
					sum=sum-1;
					if( (sum<=1) || (sum<9&&sum>3) )
						life_a[i][j]='.';
					else
						life_a[i][j]='#';
				}
				else if(sum==3)
					life_a[i][j]='#';
				else
					life_a[i][j]='.';
			}
		}
		/*------------------------------*/
		gotoxy(0,0); //印出矩陣A
		count++;
		printf(">>====%d====<<\n",count);
		for(int i=1;i<=y_side;i++){
			for(int j=1;j<=x_side;j++){
				printf("%c",life_a[i][j]);
			}
			printf("\n");
		}
		Sleep(sec);
	}
	
	return 0;
}