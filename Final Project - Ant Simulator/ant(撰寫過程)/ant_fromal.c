#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<windows.h>

#define true 1
#define false 0

#define row_side 40
#define line_side 187
#define walk_times 20
#define det_hormone 30


char map[2][row_side+2][line_side+2];	//1~side（+2用於設置地圖邊界）
										//0、side+1為邊界
int hormone[row_side+2][line_side+2];
int food[row_side+2][line_side+2];
float dis[row_side+2][line_side+2];


struct node{
    int row;
    int line;
    struct node *ptr;
};

struct node *top = NULL;


struct ant{
    int row;
    int line;
    
    int desrow;	//行動目的地
    int desline;
    
    int food;	//記錄身上是否有食物
    int walk;	//可以走幾步(生命值)
    int job;	//螞蟻的工作
    struct ant *next;	//紀錄下一隻螞蟻，原本是想用array儲存，但問題是要增加的時候用array會不好做
    struct ant *pre;	//紀錄上一隻螞蟻
};
/*
【 job list 】
	1.find food
	2.go back to home
	3.follow hormine
*/

struct ant *rear = NULL;
struct ant *front = NULL;




/*-------------------------------------*/
void gotoxy(int x, int y){//刷新頁面
	COORD pos = {x,y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}
/*-------------------------------------*/
void home(int row,int line);
void output(int);
void swap(int *a,int *b);
int search(int row,int line);

void generate_new_ant(int row,int line);
void enqueue(int row,int line);
void dequeue(struct ant *dead);

void push(int row,int line);
void pop(void);

void ant_detect(struct ant *,int,int);
void ant_normalmove(int,int,int *,int *);
double distance(int,int,int,int);
/*-------------------------------------*/


int now = 0, next = 1;	//切換矩陣
int home_row,home_line;	//蟻巢中心點


int main(){
    
    /*地圖歸零----------------------------------------------------------------------*/
    for(int i=1;i<=row_side;i++){
        for(int j=1;j<=line_side;j++){
            map[0][i][j] = ' ';
            map[1][i][j]=' ';
            hormone[i][j] = 0;
            food[i][j]=0;
        }
    }
    for(int i=0,j=0; j<line_side+2; j++){
        map[0][i][j] = '-';
        map[1][i][j] = '-';
    }  
    for(int i=row_side+1,j=0 ; j<line_side+2 ; j++){
        map[0][i][j] = '-';
        map[1][i][j] = '-';
    }
    for(int i=0,j=0; i<row_side+2; i++){
        map[0][i][j] = '|';
        map[1][i][j] = '|';
    }
    for(int i=0,j=line_side+1 ; i<row_side+2 ; i++){
        map[0][i][j] = '|';
        map[1][i][j] = '|';
    }  
    /*----------------------------------------------------------------------*/
    
/*設置蟻巢(3x3)*/
    srand(time(NULL));
    home_row = rand()%(row_side-3)+3;	//side-3以避免蟻巢超出地圖邊界
    home_line = rand()%(line_side-3)+3;
    home(home_row,home_line);
/*-------------*/
    
    for(int i=1;i<=row_side;i++){
        for(int j=1;j<=line_side;j++){
            float x,y,ans;
            x = i-home_row;
            y = j-home_line;
            
            ans = sqrt( (x*x+y*y) );
            
            dis[i][j] = ans;
        }
    }
    
/*----------------------------------------------------------------------*/
    
    int ant_count, ant_movetimes, food_count, food_times;
    int ant_len = 0;	//有幾隻螞蟻
    int tick = 1;		//第幾代
    
    printf("請問要讓螞蟻一次動幾步？ ");
    scanf("%d",&ant_movetimes);
    printf("請問一次要生成幾個食物？ ");
    scanf("%d",&food_count);
    printf("請問多久要生成一次食物？ ");
    scanf("%d",&food_times);
    
/*-------------*/
    //設置初始螞蟻
    for(int i=0,j=-1; i<5; i++){
        map[now][home_row+i][home_line+j] = '@';
        generate_new_ant(home_row+i,home_line+j);
        ant_len++;
    }
    for(int i=0,j=4; i<5; i++){
        map[now][home_row+i][home_line+j] = '@';
        generate_new_ant(home_row+i,home_line+j);
        ant_len++;
    }
    for(int i=-1,j=-1; j<5; j++){
        map[now][home_row+i][home_line+j] = '@';
        generate_new_ant(home_row+i,home_line+j);
        ant_len++;
    }
    for(int i=4,j=(-1)+1; j<5-1; j++){
        map[now][home_row+i][home_line+j] = '@';
        generate_new_ant(home_row+i,home_line+j);
        ant_len++;
    }
/*-------------*/   
    
    for(int i=0; i<food_count; i++){	//產生初始食物
        food[ rand()%(row_side)+1 ][ rand()%(line_side)+1 ] += rand()%6+1;	//食物數量：1~5
    }
    
/*----------------------------------------------------------------------*/
    output(now);//print original matric
    
    while( !feof(stdin) ){
    	
        printf("【第%d輪】\n",tick);
        tick++;
        
        if((tick%food_times) == 0){		//每過指定次數生成一次食物
            for(int i=0;i<food_count;i++){
                food[ rand()%(row_side)+1 ][ rand()%(line_side)+1 ] += rand()%6+1;
            }
        }
        
		struct ant *current = rear;
        for(int i=0; i<ant_len; i++){	//走訪每隻螞蟻
            
            (current->walk)--;
            
            if((current->walk) == 0){	//螞蟻死亡
            	struct ant *temp = current;
            	current = temp->next;
            	dequeue(temp);
            	continue;
			}
			
			ant_detect(current,current->job,ant_movetimes);
			
			current = current->next;
		}	
		
		
	/*輸出矩陣*/
        swap(&now,&next);
        
       	output(now);//print matric
    /*------------*/
	}
			
	return 0;
}


void home(int row,int line){
    for(int i=-1;i<=1;i++)
        for(int j=-1;j<=1;j++){
            map[now][row+i][line+j] = '#';
            map[next][row+i][line+j] = '#';
        }
}


void output(int list){
	for(int i=0;i<row_side+2;i++){
        for(int j=0;j<line_side+2;j++)
            printf("%c",map[list][i][j]);
        printf("\n");
    }
}


void swap(int *a,int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}


int search(int row,int line){
	for(int i=-1;i<=1;i++){
		for(int j=-1;j<=1;j++){
			if(food[row][line] > 0)
				return true;
		}
	}
	
	return false;
}


void generate_new_ant(int row,int line){
	map[now][row][line] = '@';
	enqueue(row,line);
}


void enqueue(int row,int line){
	struct ant *current = malloc(sizeof(struct ant));
	
	current->row = row;
	current->line = line;
	current->walk = walk_times;
	current->food = false;
	current->job = 1;	//初始設定螞蟻的工作為尋找食物
	
	/*設定螞蟻行動目的地*/
	do{
		current->desrow = rand()%(row_side+1)+1;
    	current->desline = rand()%(line_side+1)+1;
	}while(map[now][current->desrow][current->desline] != '#');
	
	
	if(rear == NULL){
		current->next = front;
		current->pre = rear;
		rear = front = current;
	}
	else{
		rear->pre = current;
		current->next = rear;
		current->pre = rear;
		
		rear = current;
	}
}


void dequeue(struct ant *dead){
	if(dead == rear && dead == front){
		rear = front = NULL;
		free(dead);
	}
	else if(dead == rear){
		rear = dead->next;
		dead->next->pre = NULL;
		free(dead);
	}
	else if(dead == front){
		front = dead->pre;
		dead->pre->next = NULL;
		free(dead);
	}
	else{
		dead->pre->next = dead->next;
		dead->next->pre = dead->pre;
		free(dead);
	}
}


void ant_detect(struct ant *current, int job, int ant_movetimes){	//偵測螞蟻要做甚麼工作
	int row = current->row, line = current->line;
    int max_row = -1;
    int max_line = -1;
    
    int hormone_value;	//九宮格中超過偵測值的最小賀爾蒙值
    int deal_hormone;
    
    int max_food;
    
    int len = 0;	//周圍格子中的食物為max_food的數量
    
    
    for(int count=0; count<ant_movetimes; count++){
        hormone_value = 0;
	    max_food = 0;
        deal_hormone = false;
        
        
        switch(job){
    		case 1:;	//find food
    			int row_hormone;
    			int line_hormone;
    			
    			for(int i=-1; i<=1; i++){	//掃周圍的九宮格(螞蟻的視野可見周圍九宮格有無食物)
            		for(int j=-1; j<=1; j++){
            			/*
            			if(map[next][row+i][line+i] == '@')
            				continue;
               			else if(i==0 && j==0)	//不可留在原地
                    		continue;
                        上面可寫成
                        */
                    	if(hormone[row+i][line+i] > det_hormone){	//探測賀爾蒙值是否大於det_hormone
                    		row_hormone = row+i;
                    		line_hormone = line+j;
                    		deal_hormone = true;
						}
                        	
						if(!isdigit(map[next][row+i][line+i]) )	//該格子不是食物
							continue;
						
						
					/*尋找九宮格中食物最多的格子*/
                    	if(food[row+i][line+j] > max_food){
                        	max_food = food[row+i][line+j];
                        	max_row = row+i;
                        	max_line = line+j;
                        
                        	while(top->ptr != NULL){	//當找到的格子中有更多的食物時，清空stack，並將該格存入stack中
                        		pop();
    	                    }
    	                    push(max_row,max_line);
    	                    len = 1;
    	                }
    	                else if(food[row+i][line+j] == max_food){	//食物數量相同時，存入stack
    	                    push(row+i,line+j);
    	                    len++;
    	                }
    	            }
    	        }
    	        
    	        if(max_food != 0){
    	        	if(len > 1){	//有相同數量的食物時，隨機以其中一個格子作為目的地
    	        		struct node *temp = top;
            			int decide = rand()%len;
            		
            			for(int i=0; i<decide; i++){
                			temp = temp->ptr;
            			}
           			 	max_row = temp->row;
            			max_line = temp->line;
        			}
        			
        		/*移動螞蟻到食物地點*/
        			current->row = max_row;
        			current->line = max_line;
        			current->job = 2;	//切換到go back to home
        			map[next][current->row][current->line] = '@';
        			
        			food[max_row][max_line]--;	//食物數量-1
				}
				else{	//max_food = 0
					/*是否依循賀爾蒙？*/
					if(deal_hormone == true){
					/*移動螞蟻到賀爾蒙處*/
        				current->row = row_hormone;
        				current->line = line_hormone;
        				
        				if(search(current->row,current->line) == false)
        					current->job = 3;	//周圍沒有食物，切換到follow hormone
        					
        				map[next][current->row][current->line] = '@';
					}
					else	//正常移動螞蟻
						ant_normalmove(current->desrow,current->desline,&(current->row),&(current->line));
				}
    	        
        		
        		while(top->ptr != NULL)	//清空stack
            		pop();
        		
    			break;
    			
    			
    		case 2:;	//go back to home
    			
    			struct list{
    				int row;
    				int line;
    				double distance;
				}way[3];
    			
    			/* 順序：由右往左，由上往下
    			0 1 2
    			3   4
    			5 6 7
    			*/
    			
    			if(row > home_row){
					if(line > home_line){		//螞蟻在家的右下
						way[0].row = row-1;
						way[0].line = line-1;
						way[0].distance = distance(home_row,home_line,row-1,line-1);
						
						way[1].row = row-1;
						way[1].line = line;
						way[1].distance = distance(home_row,home_line,row-1,line);
						
						way[2].row = row;
						way[2].line = line-1;
						way[2].distance = distance(home_row,home_line,row,line-1);
					}
					else if(line < home_line){	//螞蟻在家的左下
						way[0].row = row-1;
						way[0].line = line;
						way[0].distance = distance(home_row,home_line,row-1,line);
						
						way[1].row = row-1;
						way[1].line = line+1;
						way[1].distance = distance(home_row,home_line,row-1,line+1);
						
						way[2].row = row;
						way[2].line = line+1;
						way[2].distance = distance(home_row,home_line,row,line+1);
					}
					else{						//螞蟻在家的正下方
						way[0].row = row+1;
						way[0].line = line-1;
						way[0].distance = distance(home_row,home_line,row+1,line-1);
						
						way[1].row = row+1;
						way[1].line = line;
						way[1].distance = distance(home_row,home_line,row+1,line);
						
						way[2].row = row+1;
						way[2].line = line+1;
						way[2].distance = distance(home_row,home_line,row+1,line+1);
					}
				}
				else if(row < home_row){
					if(line > home_line){		//螞蟻在家的右上
						way[0].row = row;
						way[0].line = line-1;
						way[0].distance = distance(home_row,home_line,row,line-1);
						
						way[1].row = row+1;
						way[1].line = line-1;
						way[1].distance = distance(home_row,home_line,row+1,line-1);
						
						way[2].row = row+1;
						way[2].line = line;
						way[2].distance = distance(home_row,home_line,row+1,line);
					}
					else if(line < home_line){	//螞蟻在家的左上
						way[0].row = row;
						way[0].line = line+1;
						way[0].distance = distance(home_row,home_line,row,line+1);
						
						way[1].row = row+1;
						way[1].line = line;
						way[1].distance = distance(home_row,home_line,row+1,line);
						
						way[2].row = row+1;
						way[2].line = line+1;
						way[2].distance = distance(home_row,home_line,row+1,line+1);
					}
					else{						//螞蟻在家的正上方
						way[0].row = row-1;
						way[0].line = line-1;
						way[0].distance = distance(home_row,home_line,row-1,line-1);
						
						way[1].row = row-1;
						way[1].line = line;
						way[1].distance = distance(home_row,home_line,row-1,line);
						
						way[2].row = row-1;
						way[2].line = line+1;
						way[2].distance = distance(home_row,home_line,row-1,line+1);
					}
				}
				else{	//home_row = row
					if(line > home_line){		//螞蟻在家的正右方
						way[0].row = row+1;
						way[0].line = line-1;
						way[0].distance = distance(home_row,home_line,row+1,line-1);
						
						way[1].row = row;
						way[1].line = line-1;
						way[1].distance = distance(home_row,home_line,row,line-1);
						
						way[2].row = row-1;
						way[2].line = line-1;
						way[2].distance = distance(home_row,home_line,row-1,line-1);
					}
					else{	//homeline < line	//螞蟻在家的正左方
						way[0].row = row+1;
						way[0].line = line+1;
						way[0].distance = distance(home_row,home_line,row+1,line+1);
						
						way[1].row = row;
						way[1].line = line+1;
						way[1].distance = distance(home_row,home_line,row,line+1);
						
						way[2].row = row-1;
						way[2].line = line+1;
						way[2].distance = distance(home_row,home_line,row-1,line+1);
					}
				}
				
				int min_distance = 0, det = 0;	//det暫存 i 的值
				
				int i;
				for(i=0;i<3;i++){		//至多執行三次
					for(int j=0;j<3;j++){	//找到離家最近的點
						if(way[j].distance < min_distance){
							min_distance = way[j].distance;
							det = j;
						}
					}
					if(map[next][ way[det].row ][ way[det].line ] != '@'){	//判斷該位置有沒有同伴
						current->row = way[det].row;
						current->line = way[det].line;
						map[next][ way[det].row ][ way[det].line ] = '@';
						
						break;
					}
					else	//有同伴時，該位置不作為選項，並繼續找到下一個近的位置
						way[det].distance = 0.0;
				}
				
				if(i == 2){
					map[next][row][line] = '@';			//若三個位置都有同伴，則留在原地
				}
				else{	//有移動過位置，判斷是否抵達家中
					if( abs(row - home_row) <= 2){
						if( abs(line - home_line) <= 2){	//抵達家的四週
							current->job = 1;
							do{							//重新設定目的地
								current->desrow = rand()%(row_side+1)+1;
    							current->desline = rand()%(line_side+1)+1;
							}while(map[now][current->desrow][current->desline] != '#');
						}
					}
				}
				
    			break;
    			
    		case 3:;	//follow hormone
    		
    			break;
    		default:
    			break;
		}
	}
}


void push(int row,int line){
	struct node *temp = malloc(sizeof(struct node));
	
	temp->row = row;
	temp->line = line;
	
	if(top == NULL){
		temp->ptr = NULL;
		top = temp;
	}
	else{
		temp->ptr = top;
		top = temp;
	}
}


void pop(void){
	if(top != NULL){
		struct node *temp = top;
		top = temp->ptr;
		free(temp);
	}
}


void ant_normalmove(int desrow,int desline,int *row,int *line){
	double lenth[3][3];
	int min_row, min_line;
	for(int i=-1;i<=1;i++){	//測量九宮格每格與目的地的距離
		for(int j=-1;j<=1;j++){
			if(i == 0 && j == 0)
				continue;
			
			lenth[i+1][j+1] == distance(desrow,desline,*row,*line);
		}
	}
	
	while(true){	//找最小距離
		double min_lenth = -1;
		min_row = *row-1, min_line = *line-1;
		
		for(int i=-1;i<=1;i++){
			for(int j=-1;j<=1;j++){
				if(i == 0 && j == 0)
					continue;
				
				if(lenth[i+1][j+1] < min_lenth){
					min_lenth = lenth[i+1][j+1];
					min_row = *row+i;
					min_line = *line+j;
				}
			}
		}
		
		if(map[next][min_row][min_line] != '@')	//該格沒有同伴才可跳出活圈，否則繼續找到下一格
			break;
		else
			lenth[min_row][min_line] = 0;
	}
	
	map[next][min_row][min_line] = '@';
	*row = min_row;
	*line = min_line;
	/*
	if(desrow > *row){
		
		if(desline > *line){
			
		}
		else if(desline < *line){
			
		}
		else{
			
		}
	}
	else if(desrow < *row){
		
		if(desline > *line){
			
		}
		else if(desline < *line){
			
		}
		else{
			
		}
	}
	else{	//desrow = *row
		if(desline > *line){
			
		}
		else{	//desline < *line
			
		}
	}
	*/
}


double distance(int A,int B,int a,int b){
	return sqrt( (A-a)*(A-a) + (B-b)*(B-b) );
}
