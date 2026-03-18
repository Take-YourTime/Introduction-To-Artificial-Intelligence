#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define true 1
#define false 0

#define row_side 100
#define line_side 187

//螞蟻回巢的function done
//生成食物function done
//回巢生新的螞蟻的function done
//main function要弄到好 done
// time interval不是很確定是什麼沒弄

char map[2][row_side+2][line_side+2];//1~side
int hormone[row_side+2][line_side+2];
int food[row_side+2][line_side+2];

int r[2];//因應function不能一次回傳兩個值，那就乾脆要一個array來接pop出來的兩個座標來做

struct node{
    int row;
    int line;
    struct node *ptr;
};

struct ant{//做成stack就行
    int row;
    int line;
    int food;//記錄身上是否有食物
    int live;//記錄存活狀態 false為死亡，true為活著
    int walk;//記錄走了幾步
    int job;//不是很確定要幹嘛，作業說明中有提到的，先放進來，沒有要用再刪掉就行
    struct ant *next;//紀錄下一隻螞蟻，原本是想用array儲存，但問題是要增加的時候用array會不好做
};


struct node *top = NULL;
struct ant *ant_top = NULL;


void push(int r,int l){
    struct node *new;
    new = malloc(sizeof(struct node));
    new->row = r;
    new->line = l;
    new->ptr = top;
    top = new;
}


int pop(void){//有時候會有點問題，但我還沒抓到 
    
	if(top == NULL)
        return 0;
    
    r[0] = top->row;
    r[1] = top->line;
    top = top->ptr;
    return 0;
}


void generate_new_ant(int len,int now){
    struct ant *new;
    new = malloc(sizeof(struct ant));
    
    if(len == 0){
        ant_top = new;
	}
	
    while(true){
        new->row = rand()%(row_side-2)+1;
        new->line = rand()%(line_side-2)+1;
        
        if(map[now][new->row][new->line] != '@'){//要生成螞蟻的位置沒有螞蟻才可以生成
            map[now][new->row][new->line] = '@';
            break;
        }
    }
    
    new->food = 0;
    new->live = true;
    new->walk = 0;
    new->next = ant_top;
    ant_top = new;
}


void ant_move(struct ant *ptr,int do_times,int now){//now是原本的array
    int row,line;
    row = ptr->row;
    line = ptr->line;
    //do-times決定螞蟻一次可以動幾步
    int next=1;//next 是要放進去的新array
    if(now==1){
        next=0;
    }
    
    int max_hormone;
    int max_row=-1;
    int max_line=-1;
    
    int deal_hermone=1;
    
    int max_food;
    
    int len=0;
    
    
    for(int count=0;count<do_times;count++){
        max_hormone = 0;
        
        for(int i=-1;i<=1;i++){
            max_food=0;
            for(int j=-1;j<=1;j++){//掃周圍的九宮格
                if(i==0&&j==0)//不可留在原地
                    continue;
                else{
                    if(map[now][row+i][line+i]=='@')
                        continue;
                        
                    else if(food[row+i][line+j]>max_food){
                        deal_hermone=0;
                        max_food=food[row+i][line+j];
                        max_row=row+i;
                        max_line=line+j;
                        
                        len=0;
                        /*沒清空*/
                        top = NULL;//可能還是要清每一個吧，
                    }
                    
                    else if(food[row+i][line+j] == max_food){
                        push(row+i,line+j);
                        len++;
                    }
                }
            }
        }
        
        if(len>0){
            int decide;
            decide=rand()%len;
            for(int i=1;i<=decide;i++){
                pop();
            }
            max_row=r[0];
            max_line=r[1];
            food[max_row][max_line]--;
        }
        
        len=0;
        top=NULL;
        if(deal_hermone == true){
            for(int i=-1;i<=1;i++){
                for(int j=-1;j<=1;j++){//掃周圍的九宮格
                    if(i==0&&j==0){//不可留在原地
                        continue;
                    }
                    else{
                        if(map[now][row+i][line+j]=='@'){
                            continue;
                        }
                        if(hormone[row+i][line+j]>max_hormone){
                            max_hormone=hormone[row+i][line+j];
                            top=NULL;
                            max_row=row+i;
                            max_line=line+j;
                        }
                        else if(hormone[row+i][line+j]==max_hormone){
                            push(row+i,line+j);
                            len++;
                        }
                    }
                }
            }
        }
        
        if(len>0){
            int decide = rand()%len;
            printf("a%d %d\n",len,decide);
            for(int i=1;i<=decide;i++)
                pop();
                
            max_row = r[0];
            max_line = r[1];
        }
        
        hormone[row][line]+=1;//走過皆會留下強度為一的賀爾蒙
        
        if(!deal_hermone){
            hormone[row][line]=hormone[row][line]+5+food[max_row][max_line];
            ptr->food+=1;
            ptr->walk=0;
        }
        
        map[next][max_row][max_line]='@';//先用at當作螞蟻
        ptr->row=max_row;
        ptr->line=max_line;
        map[now][row][line]=' ';//原本的位置要變成空白
        ptr->walk+=1;
    }
}

int backhome(struct ant *ptr,int home_row,int home_line,int do_times,int now){
    //因為底下的巢看起來是找一個點，然後往左下找3*3的空間來作為巢，所以要左上的座標就行
    //do_times決定一次可以動幾步
    
    int row,line;
    row = ptr->row;
    line = ptr->line;
    
    int next = 1;//next 是要放進去的新array
    
    if(now == 1){
        next = 0;
    }
    
    (ptr->walk)++;
    
    for(int count=0;count<do_times;count++){
        if(row > (home_row+2)){
            if(map[now][row-1][line] == '@'){//檢查是否被同伴擋住
                if(line > (home_line+2)){//被擋住就試著上下走
                    if(map[now][row][line-1] != '@'){//沒有被黨就可以走
                        map[now][row][line]=' ';
                        map[next][row][line-1]='@';
                        ptr->line=line-1;
                    }
                }
                else if(line<home_line){
                    if(map[now][row][line+1]!='@'){
                        map[now][row][line]=' ';
                        map[next][row][line+1]='@';
                        ptr->line=line+1;
                    }
                }
            }
            else{//不是就正常走
                map[now][row][line]=' ';
                map[next][row-1][line]='@';
                ptr->row=row-1;
            }
        }
        else if(row<home_row){
            if(map[now][row+1][line]=='@'){//檢查是否被同伴擋住
                if(line>home_line+2){//被擋住就試著上下走
                    if(map[now][row][line-1]!='@'){//沒有被黨就可以走
                        map[now][row][line]=' ';
                        map[next][row][line-1]='@';
                        ptr->line=line-1;
                    }
                }
                else if(line<home_line){
                    if(map[now][row][line+1]!='@'){
                        map[now][row][line]=' ';
                        map[next][row][line+1]='@';
                        ptr->line=line+1;
                    }
                }
            }
            else{//不是就正常走
                map[now][row][line]=' ';
                map[next][row+1][line]='@';
                ptr->row=row+1;
            }
        }
        else if (line>home_line+2){
            if(map[now][row][line-1]=='@'){//檢查是否被同伴擋住
                if(row>home_row+2){//被擋住就試著左右走
                    if(map[now][row-1][line]!='@'){//沒有被黨就可以走
                        map[now][row][line]=' ';
                        map[next][row-1][line]='@';
                        ptr->row=row-1;
                    }
                }
                else if(row<home_row){
                    if(map[now][row+1][line]!='@'){
                        map[now][row][line]=' ';
                        map[next][row+1][line]='@';
                        ptr->row=row+1;
                    }
                }
            }
            else{//不是就正常走
                map[now][row][line]=' ';
                map[next][row][line-1]='@';
                ptr->line=line-1;
            }
        }
        else if(line<home_line){
            if(map[now][row][line+1]=='@'){//檢查是否被同伴擋住
                if(row>home_row+2){//被擋住就試著左右走
                    if(map[now][row-1][line]!='@'){//沒有被黨就可以走
                        map[now][row][line]=' ';
                        map[next][row-1][line]='@';
                        ptr->row=row-1;
                    }
                }
                else if(row<home_line){
                    if(map[now][row+1][line]!='@'){
                        map[now][row][line]=' ';
                        map[next][row+1][line]='@';
                        ptr->row=row+1;
                    }
                }
            }
            else{//不是就正常走
                map[now][row][line]=' ';
                map[next][row][line+1]='@';
                ptr->line=line+1;
            }
        }
        hormone[row][line]+=5;//因為身上有食物，所以在路徑上留下賀爾蒙
        //判斷到家
        if(ptr->row<=home_row+2 && ptr->row>=home_row&&ptr->line<=home_line+2&&ptr->line>=home_line){
            //要生成新的螞蟻，然後出門
            //出門理論上是要從巢穴出門，但實作上非常麻煩，假如有兩隻螞蟻同時回巢的話就不能固定出來的位置，然後往固定方向移動的話也會有撞到邊角的問題，所以我們假設蟻巢有高科技迫擊砲，能夠安全的把螞蟻投射到隨機一個沒有螞蟻的點，當然是因為趕著儘快寫出來，之後假如有餘裕的話我會把這段優化好
            while(1){
                ptr->row=rand()%(row_side-2)+1;
                ptr->line=rand()%(line_side-2)+1;
                if(map[now][ptr->row][ptr->line]!='@'){//要生成螞蟻的位置沒有螞蟻才可以生成
                    map[now][ptr->row][ptr->line]='@';
                    break;
                }
            }
            ptr->walk=0;
            return 1;//表示需要生成新的螞蟻
        }
        
    }
    
    return 0;//表示還沒回巢
}


void home(int,int);


int main(){
    system("pause");
    int home_row,home_line;
    
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
        
    
    /*設置蟻巢(3x3)*/
    srand(time(NULL));
    home_row = rand()%(row_side-2)+1;
    home_line = rand()%(line_side-2)+1;
    home(home_row,home_line );
    /*-------------*/
    
    /*----------------------------------------------------------------------*/
    int ant_count,ant_movetimes,food_count,food_times,times,death;
    int ant_len=0;
/*$*/	int now=0;
    
    printf("請問要生成幾隻螞蟻？ ");
    scanf("%d",&ant_count);
    printf("請問要讓螞蟻一次動幾步？ ");
    scanf("%d",&ant_movetimes);
    printf("請問一次要生成幾個食物？ ");
    scanf("%d",&food_count);
    printf("請問多久要生成一次食物？ ");
    scanf("%d",&food_times);
    printf("請問螞蟻要走幾步就會死？ ");
    scanf("%d",&death);
    printf("請問要模擬幾次？ ");
    scanf("%d",&times);
    
    for(int i=0;i<ant_count;i++){
        generate_new_ant(ant_len,now);
        ant_len++;
    }
    
    for(int i=0;i<food_count;i++){
        food[ rand()%(row_side-2)+1 ][ rand()%(line_side-2)+1 ] = 1;
    }
    /*----------------------------------------------------------------------*/
    for(int tick=1; tick<=times; tick++){
        printf("【第%d輪】\n",tick);
        
        if((tick%food_times) == 0){//每過指定次數生成一次食物
            for(int i=0;i<food_count;i++){
                food[ rand()%(row_side-2)+1 ][ rand()%(line_side-2)+1 ] += 1;
            }
        }
        
        for(int i=0;i<ant_len;i++){
            struct ant *current = ant_top;
            
            if(current->live == true){
                if(current->food > 0){
                    int ge;
                    ge = backhome(current,home_row,home_line,ant_movetimes,now);
                    if(ge == true){
                        generate_new_ant(ant_len,now);
                        ant_len++;
                    }
                }
                else{
                    ant_move(current,ant_movetimes,now);
                }
            }
            if(current->walk>death){
                current->live=0;
            }
            current=current->next;//最後會變NULL
        }
        
        if(now==0)
            now=1;
        else
            now=0;
        for(int i=0;i<row_side+2;i++){
            for(int j=0;j<line_side+2;j++)
                printf("%c",map[now][i][j]);
            printf("\n");
        }
    }
    return 0;
}


void home(int row,int line){
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            map[0][row+i][line+j] = '#';
}
