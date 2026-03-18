#include<stdio.h>
#include<windows.h>
#define true 1
#define false 0
#define x_side 80 //x軸長度
#define y_side 40 //y軸長度
#define sec 100

void gotoxy(int x, int y){//刷新頁面
	COORD pos = {x,y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

int main(){
	int x=0,y=0;//座標，用來設定初始存活細胞的位置
	int sum;//某細胞周圍存活細胞的數量
	int count=1;//年代
	int mode=0;//模式
	char stop;//stop用來暫停程式
	char life_a[y_side+1][x_side+1],life_b[y_side+1][x_side+1];//(x_side+1)x(y_side+1)
	
	start:
		
	printf("請選擇模式！\n");
    printf("輸入1為自訂模式，2為高斯帕機槍持續製造滑翔機模式，3為中山大學生態平衡狀態\n");
    
    scanf("%d",&mode);
    for(int i=0;i<=y_side;i++){//歸零
        for(int j=0;j<=x_side;j++){
            life_a[i][j]='.';
            life_b[i][j]='.';
        }
    }
    
    if(mode==1){//自訂模式
    	fflush(stdin);
        while(scanf("%d%d",&x,&y)){//初始位置
            if(x==-1)//輸入-1 → 跳出
                break;
            x--; y--;//化為座標，方便閱讀
            life_a[x][y]='#';
        }
    }
    else if(mode==2){//高帕斯機槍持續製造滑翔機
        life_a[0][25]='#';
        life_a[1][22]='#';life_a[1][23]='#';life_a[1][24]='#';life_a[1][25]='#';life_a[1][30]='#';
        life_a[2][13]='#';life_a[2][21]='#';life_a[2][22]='#';life_a[2][23]='#';life_a[2][24]='#';life_a[2][30]='#';
        life_a[3][12]='#';life_a[3][14]='#';life_a[3][21]='#';life_a[3][24]='#';life_a[3][34]='#';life_a[3][35]='#';
        life_a[4][11]='#';life_a[4][15]='#';life_a[4][16]='#';life_a[4][21]='#';life_a[4][22]='#';life_a[4][23]='#';life_a[4][24]='#';life_a[4][34]='#';life_a[4][35]='#';
        life_a[5][0]='#';life_a[5][1]='#';life_a[5][11]='#';life_a[5][15]='#';life_a[5][16]='#';life_a[5][22]='#';life_a[5][23]='#';life_a[5][24]='#';life_a[5][25]='#';
        life_a[6][0]='#';life_a[6][1]='#';life_a[6][11]='#';life_a[6][15]='#';life_a[6][16]='#';life_a[6][25]='#';
        life_a[7][12]='#';life_a[7][14]='#';
        life_a[8][13]='#';
    }
    else if(mode==3){
        life_a[0][3]='#';
        life_a[0][17]='#';
        life_a[1][1]='#';
        life_a[1][2]='#';
        life_a[1][3]='#';
        life_a[1][15]='#';
        life_a[1][16]='#';
        life_a[1][17]='#';
        life_a[2][0]='#';
        //life_a[2][7]='#';
        //life_a[2][8]='#';
        life_a[2][9]='#';
        life_a[2][10]='#';
        //life_a[2][11]='#';
        life_a[2][14]='#';
        life_a[3][1]='#';
        life_a[3][2]='#';
        life_a[3][3]='#';
        life_a[3][8]='#';
        life_a[3][11]='#';
        life_a[3][15]='#';
        life_a[3][16]='#';
        life_a[3][17]='#';
        life_a[4][4]='#';
        life_a[4][8]='#';
        life_a[4][11]='#';
        life_a[4][18]='#';
        life_a[5][1]='#';
        life_a[5][2]='#';
        life_a[5][3]='#';
        life_a[5][9]='#';
        life_a[5][10]='#';
        life_a[5][15]='#';
        life_a[5][16]='#';
        life_a[5][17]='#';
        life_a[6][1]='#';
        life_a[6][15]='#';
    }
    else{
    	printf("無效的輸入\n");
    	goto start;//輸入錯誤時重新輸入
	}
	/*------------------------------*/
	gotoxy(0,0);
	printf(">>====%d====<<\n",count);//輸出原始位置
	for(int i=0;i<=y_side;i++){
		for(int j=0;j<=x_side;j++){
			printf("%c",life_a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	Sleep(sec);
	/*------------------------------*/
	
	/*(j,i) (n,k)*/
	while(true){
		sum=0;
		for(int i=0;i<2;i++)//四個角落(0,0)
			for(int j=0;j<2;j++)
				if(life_a[i][j]=='#')
					sum++;
				//sum=0~3
		if(life_a[0][0]=='#'){
			sum=sum-1;
			if(sum<=1)
				life_b[0][0]='.';
			else
				life_b[0][0]='#';
		}
		else if(sum==3)
			life_b[0][0]='#';
		else
			life_b[0][0]='.';
		/*------------------------------*/
		sum=0;
		for(int i=0;i<2;i++)//四個角落(side,0)
			for(int j=x_side-1;j<=x_side;j++)
				if(life_a[i][j]=='#')
					sum++;
				//sum=0~3
		if(life_a[0][x_side]=='#'){
			sum=sum-1;
			if(sum<1)
				life_b[0][x_side]='.';
			else
				life_b[0][x_side]='#';
		}
		else if(sum==3)
			life_b[0][x_side]='#';
		else
			life_b[0][x_side]='.';
		/*------------------------------*/
		sum=0;
		for(int i=y_side-1;i<=y_side;i++)//四個角落(0,side)
			for(int j=0;j<2;j++)
				if(life_a[i][j]=='#')
					sum++;
					//sum=0~3
		if(life_a[y_side][0]=='#'){
			sum=sum-1;
			if(sum<1)
				life_b[y_side][0]='.';
			else
				life_b[y_side][0]='#';
		}
		else if(sum==3)
			life_b[y_side][0]='#';
		else
			life_b[y_side][0]='.';
		/*------------------------------*/
		sum=0;
		for(int i=y_side-1;i<=y_side;i++)//四個角落(side,side)
			for(int j=x_side-1;j<=x_side;j++)
				if(life_a[i][j]=='#')
					sum++;
					//sum=0~3
		if(life_a[y_side][x_side]=='#'){
			sum=sum-1;
			if(sum<1)
				life_b[y_side][x_side]='.';
			else
				life_b[y_side][x_side]='#';
		}
		else if(sum==3)
			life_b[y_side][x_side]='#';
		else
			life_b[y_side][x_side]='.';
		/*------------------------------*/
		for(int i=1,j=0;i<=y_side-1;i++){//左邊界
			sum=0;
			for(int k=i-1;k<=i+1;k++)
				for(int n=j;n<=j+1;n++)
					if(life_a[k][n]=='#')
						sum++;
							
			if(life_a[i][j]=='#'){
				sum=sum-1;
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
		/*------------------------------*/
		for(int i=1,j=x_side;i<=y_side-1;i++){//右邊界
			sum=0;
			for(int k=i-1;k<=i+1;k++)
				for(int n=j-1;n<=j;n++)
					if(life_a[k][n]=='#')
						sum++;
							
			if(life_a[i][j]=='#'){
				sum=sum-1;
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
		/*------------------------------*/
		for(int i=0,j=1;j<=x_side-1;j++){//上邊界
			sum=0;
			for(int k=i;k<=i+1;k++)
				for(int n=j-1;n<=j+1;n++)
					if(life_a[k][n]=='#')
						sum++;
							
			if(life_a[i][j]=='#'){
				sum=sum-1;
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
		/*------------------------------*/
		for(int i=y_side,j=1;j<=x_side-1;j++){//下邊界
			sum=0;
			for(int k=i-1;k<=i;k++)
				for(int n=j-1;n<=j+1;n++)
					if(life_a[k][n]=='#')
						sum++;
							
			if(life_a[i][j]=='#'){
				sum=sum-1;
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
		/*------------------------------*/
		for(int i=1;i<=y_side-1;i++){//中間的部分
			for(int j=1;j<=x_side-1;j++){
				sum=0;
				for(int k=i-1;k<=i+1;k++)
					for(int n=j-1;n<=j+1;n++)
						if(life_a[k][n]=='#')
							sum++;
							
				if(life_a[i][j]=='#'){
					sum=sum-1;
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
		gotoxy(0,0);
		count++;
		printf(">>====%d====<<\n",count);
		for(int i=0;i<=y_side;i++){
			for(int j=0;j<=x_side;j++){
				printf("%c",life_b[i][j]);
			}
			printf("\n");
		}
		Sleep(sec);
		
		/*------------------------------------------------------------*/
		
		/*------------------------------------------------------------*/
		
		sum=0;
		for(int i=0;i<2;i++)//四個角落(0,0)
			for(int j=0;j<2;j++)
				if(life_b[i][j]=='#')
					sum++;
				//sum=0~3
		if(life_a[0][0]=='#'){
			sum=sum-1;
			if(sum<1)
				life_a[0][0]='.';
			else
				life_a[0][0]='#';
		}
		else if(sum==3)
			life_a[0][0]='#';
		else
			life_a[0][0]='.';
		/*------------------------------*/
		sum=0;
		for(int i=0;i<2;i++)//四個角落(side,0)
			for(int j=x_side-1;j<=x_side;j++)
				if(life_b[0][x_side]=='#')
					sum++;
				//sum=0~3
		if(life_b[0][x_side]=='#'){
			sum=sum-1;
			if(sum<1)
				life_a[0][x_side]='.';
			else
				life_a[0][x_side]='#';
		}
		else if(sum==3)
			life_a[0][x_side]='#';
		else
			life_a[0][x_side]='.';
		/*------------------------------*/
		sum=0;
		for(int i=y_side-1;i<=y_side;i++)//四個角落(0,side)
			for(int j=0;j<2;j++)
				if(life_b[y_side][0]=='#')
					sum++;
					//sum=0~3
		if(life_b[y_side][0]=='#'){
			sum=sum-1;
			if(sum<1)
				life_a[y_side][0]='.';
			else
				life_a[y_side][0]='#';
		}
		else if(sum==3)
			life_a[y_side][0]='#';
		else
			life_a[y_side][0]='.';
		/*------------------------------*/
		sum=0;
		for(int i=y_side-1;i<=y_side;i++)//四個角落(side,side)
			for(int j=x_side-1;j<=x_side;j++)
				if(life_b[y_side][x_side]=='#')
					sum++;
					//sum=0~3
		if(life_b[y_side][x_side]=='#'){
			sum=sum-1;
			if(sum<1)
				life_a[y_side][x_side]='.';
			else
				life_a[y_side][x_side]='#';
		}
		else if(sum==3)
			life_a[y_side][x_side]='#';
		else
			life_a[y_side][x_side]='.';
		/*------------------------------*/
		for(int i=1,j=0;i<=y_side-1;i++){//左邊界
			sum=0;
			for(int k=i-1;k<=i+1;k++)
				for(int n=j;n<=j+1;n++)
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
		/*------------------------------*/
		for(int i=1,j=x_side;i<=y_side-1;i++){//右邊界
			sum=0;
			for(int k=i-1;k<=i+1;k++)
				for(int n=j-1;n<=j;n++)
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
		/*------------------------------*/
		for(int i=0,j=1;j<=x_side-1;j++){//上邊界
			sum=0;
			for(int k=i;k<=i+1;k++)
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
		/*------------------------------*/
		for(int i=y_side,j=1;j<=x_side-1;j++){//下邊界
			sum=0;
			for(int k=i-1;k<=i;k++)
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
		/*------------------------------*/
		for(int i=1;i<=y_side-1;i++){//中間的部分
			for(int j=1;j<=x_side-1;j++){
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
		gotoxy(0,0);
		count++;
		printf(">>====%d====<<\n",count);
		for(int i=0;i<=y_side;i++){
			for(int j=0;j<=x_side;j++){
				printf("%c",life_a[i][j]);
			}
			printf("\n");
		}
		Sleep(sec);
	}
	
	return 0;
}
