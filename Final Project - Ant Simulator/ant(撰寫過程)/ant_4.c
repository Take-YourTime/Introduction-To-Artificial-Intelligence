#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<windows.h>

#define true 1
#define false 0

#define row_side 40
#define line_side 187
#define walk_time 20
#define do_hormone 30
char map[2][row_side+2][line_side+2];//1~side
int hormone[row_side+2][line_side+2];
int food[row_side+2][line_side+2];
int dis[row_side+2][line_side+2];

int r[2];//因應function不能一次回傳兩個值，那就乾脆要一個array來接pop出來的兩個座標來做

struct node{
    int row;
    int line;
    struct node *ptr;
};

struct ant{//做成stack就行
    int row;
    int line;
    int desrow;
    int desline;
    int food;//記錄身上是否有食物
    int walk;
    int job;//不是很確定要幹嘛，作業說明中有提到的，先放進來，沒有要用再刪掉就行
    struct ant *next;//紀錄下一隻螞蟻，原本是想用array儲存，但問題是要增加的時候用array會不好做
};


struct node *top = NULL;
struct ant *ant_top = NULL;


/*-------------------------------------*/
void gotoxy(int x, int y){//刷新頁面
	COORD pos = {x,y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}
/*-------------------------------------*/


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


void generate_new_ant(int len,int now,int row,int line){
    struct ant *new;
    new = malloc(sizeof(struct ant));
    
    if(len == 0){
        ant_top = new;
    }
    new->row=row;
    new->line=line;
    new->food = 0;
    new->walk = walk_time;
    new->next = ant_top;
    new->desrow=rand()%(row_side-4)+2;
    new->desline=rand()%(line_side-4)+2;
    ant_top = new;
}


void ant_move(struct ant *ptr,int do_times,int now){//now是原本的array
    int row,line;
    row = ptr->row;
    line = ptr->line;
    //do-times決定螞蟻一次可以動幾步
    int next=1;//next 是要放進去的新array
    if(now == 1){
        next = 0;
    }
    
    int max_hormone=do_hormone;
    int max_row = -1;
    int max_line = -1;
    
    int deal_hermone = 1;
    int deal_normal=0;
    
    int max_food;
    
    int len=0;
    
    
    for(int count=0; count<do_times; count++){
        max_hormone = 0;
        for(int i=-1;i<=1;i++){
            max_food=0;
            for(int j=-1;j<=1;j++){//掃周圍的九宮格(螞蟻的視野可見周圍九宮格有無食物)
                if(i==0 && j==0)//不可留在原地
                    continue;
                else{
                    if(map[next][row+i][line+i] == '@')
                        continue;
                        
                    else if(food[row+i][line+j] > max_food){
                        deal_hermone = 0;
                        max_food = food[row+i][line+j];
                        max_row = row+i;
                        max_line = line+j;
                        
                        while(top!=NULL){
                            struct node *temp;
                            temp=top;
                            top=top->ptr;
                            free(temp);
                        }
                        free(top);
                        len = 0;
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
            decide = rand()%len+1;
            for(int i=1;i<=decide;i++){
                pop();
            }
            max_row = r[0];
            max_line = r[1];
            food[max_row][max_line]--;
        }
        while(top!=NULL){
            struct node *temp;
            temp=top;
            top=top->ptr;
            free(temp);
        }
        len = 0;
        top = NULL;//可能還是要清每一個吧，
        int k=0;
        int distance[8][3];
        if(deal_hermone == true){
            for(int i=-1;i<=1;i++){
                for(int j=-1;j<=1;j++){//掃周圍的九宮格
                    if(i==0 && j==0){//不可留在原地
                        continue;
                    }
                    else{
                        distance[k][0]=dis[row+i][line+j];
                        distance[k][1]=row+i;
                        distance[k][2]=line+j;
                        if(map[now][row+i][line+j] == '@'){
                            distance[k][0]=-1;
                            continue;
                        }
                        if(hormone[row+i][line+j]>max_hormone){
                            deal_normal=1;
                           
                        }
                    }
                }
            }
        }

        if (deal_normal==true) {
            for(int i=0;i<8;i++){
                for(int j=1;j<8;j++){
                    if(distance[i][0]>distance[j][0]){
                        int temp1,temp2,temp3;
                        temp1=distance[i][0];
                        distance[i][0]=distance[j][0];
                        distance[j][0]=temp1;
                        temp2=distance[i][1];
                        distance[i][1]=distance[j][1];
                        distance[j][1]=temp2;
                        temp3=distance[i][2];
                        distance[i][2]=distance[j][2];
                        distance[j][2]=temp3;
                    }
                }
            }
            for(int i=0;i<8;i++){
                if(distance[i][0]!=-1){
                    map[next][distance[i][1]][distance[i][2]]='@';
                    map[now][row][line]=' ';
                    break;
                }
            }
        }
        else{
            if(row > ptr->desrow){
                if(map[now][row-1][line] == '@'){//檢查是否被同伴擋住
                    if(line > (ptr->desline+2)){//被擋住就試著上下走
                        if(map[next][row][line-1] != '@'){//沒有被黨就可以走
                            map[now][row][line]=' ';
                            map[next][row][line-1]='@';
                            ptr->line=line-1;
                        }
                    }
                    else if(line<ptr->desline){
                        if(map[next][row][line+1]!='@'){
                            map[now][row][line]=' ';
                            map[next][row][line+1]='@';
                            ptr->line = line+1;
                        }
                    }
                }
                else{//不是就正常走
                    map[now][row][line]=' ';
                    map[next][row-1][line]='@';
                    ptr->row=row-1;
                }
            }
            else if(row < ptr->desrow){
                if(map[next][row+1][line]=='@'){//檢查是否被同伴擋住
                    if(line>ptr->desline){//被擋住就試著上下走
                        if(map[next][row][line-1]!='@'){//沒有被黨就可以走
                            map[now][row][line]=' ';
                            map[next][row][line-1]='@';
                            ptr->line=line-1;
                        }
                    }
                    else if(line<ptr->desline){
                        if(map[next][row][line+1]!='@'){
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
            else if(line>ptr->desline){//正上方
                if(map[next][row][line-1]!='@'){//檢查是否被同伴擋住
                    if(row>ptr->desrow){//被擋住就試著左右走
                        map[now][row][line]=' ';
                        map[next][row][line-1]='@';
                        ptr->line=line-1;
                    }
                }
            }
            else if(line<ptr->desline){//正下方 (執行不到)
                if(map[next][row][line+1]=='@'){//檢查是否被同伴擋住
                    map[now][row][line]=' ';
                    map[next][row][line+1]='@';
                    ptr->line=line+1;
                }
            }
        }

        if(map[now][row][line]=='@'){//留在原地的狀況
            map[next][row][line]='@';
            map[now][row][line]=' ';
        }
        else if(!deal_normal){
            ptr->walk-=1;
        }
        else{
            hormone[row][line]=10;//走過皆會留下強度為十的賀爾蒙

            if(!deal_hermone){
                hormone[row][line]=50;
                ptr->food += 1;
                ptr->walk = walk_time;
            }
            
            map[next][max_row][max_line]='@';
            ptr->row=max_row;
            ptr->line=max_line;
            map[now][row][line]=' ';//原本的位置要變成空白
            ptr->walk-=1;
        }
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
    
    (ptr->walk)--;
    
    for(int count=0;count<do_times;count++){
        if(row > (home_row+2)){
            if(map[now][row-1][line] == '@'){//檢查是否被同伴擋住
                if(line > (home_line+2)){//被擋住就試著上下走
                    if(map[next][row][line-1] != '@'){//沒有被黨就可以走
                        map[now][row][line]=' ';
                        map[next][row][line-1]='@';
                        ptr->line=line-1;
                    }
                }
                else if(line<home_line){
                    if(map[next][row][line+1]!='@'){
                        map[now][row][line]=' ';
                        map[next][row][line+1]='@';
                        ptr->line = line+1;
                    }
                }
            }
            else{//不是就正常走
                map[now][row][line]=' ';
                map[next][row-1][line]='@';
                ptr->row=row-1;
            }
        }
        else if(row < home_row){
            if(map[next][row+1][line]=='@'){//檢查是否被同伴擋住
                if(line>home_line+2){//被擋住就試著上下走
                    if(map[next][row][line-1]!='@'){//沒有被黨就可以走
                        map[now][row][line]=' ';
                        map[next][row][line-1]='@';
                        ptr->line=line-1;
                    }
                }
                else if(line<home_line){
                    if(map[next][row][line+1]!='@'){
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
        else if(line>home_line+2){//正上方
            if(map[next][row][line-1]!='@'){//檢查是否被同伴擋住
                if(row>home_row+2){//被擋住就試著左右走
                    map[now][row][line]=' ';
                    map[next][row][line-1]='@';
                    ptr->line=line-1;
                }
            }
        }
        else if(line<home_line){//正下方 (執行不到)
            if(map[next][row][line+1]=='@'){//檢查是否被同伴擋住
                map[now][row][line]=' ';
                map[next][row][line+1]='@';
                ptr->line=line+1;
            }
        }
        /*新增沒有動的處理狀況*/
        if(map[now][row][line]=='@'){
            map[next][row][line]='@';
            map[now][row][line]='@';
            ptr->walk++;
        }
        hormone[row][line] = 50;//因為身上有食物，所以在路徑上留下賀爾蒙
        //判斷到家
        if( (ptr->row <= home_row+2) && (ptr->row >= home_row) && (ptr->line <= home_line+2) && (ptr->line >= home_line) ){
            ptr->walk =walk_time;
            while(true){
                int i=1;
                if(map[next][home_row+i][home_line+1]!='@'){
                    ptr->row=home_row+i;
                    ptr->line=home_line+1;
                    break;
                }
                i++;
            }
            return 1;//表示需要生成新的螞蟻
        }
        
    }
    return 0;//表示還沒回巢
}

void home(int,int);


int main(){
    system("pause");
    int home_row,home_line;
    
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
        
    
/*設置蟻巢(3x3)*/
    //蟻巢周圍設置初始螞蟻
    srand(time(NULL));
    home_row = rand()%(row_side-4)+2;//side-3以避免蟻巢超出地圖邊界
    home_line = rand()%(line_side-4)+2;
    home(home_row,home_line );
    /*-------------*/
    for(int i=1;i<=row_side;i++){
        for(int j=1;j<=line_side;j++){
            double x,y,ans;
            x=i-home_row;
            x=x*x;
            y=j-home_line;
            y=y*y;
            x+=y;
            ans=sqrt(x);
            dis[i][j] = (int)ans;
        }
    }
    /*----------------------------------------------------------------------*/
    
    int ant_movetimes,food_count,food_times,times;
    int ant_len = 0;
    int now = 0;
    int next=1;
    
    printf("請問要讓螞蟻一次動幾步？ ");
    scanf("%d",&ant_movetimes);
    printf("請問一次要生成幾個食物？ ");
    scanf("%d",&food_count);
    printf("請問多久要生成一次食物？ ");
    scanf("%d",&food_times);
    printf("請問要模擬幾次？ ");
    scanf("%d",&times);
	
    //初始螞蟻位於蟻巢周圍
    for(int i=-1,j=-1; i<3; i++){
        map[0][home_row+i][home_line+j] = '@';
        generate_new_ant(ant_len,now,home_row+i,home_line+j);
        ant_len++;
    }
    for(int i=-1,j=4; i<3; i++){
        map[0][home_row+i][home_line+j] = '@';
        generate_new_ant(ant_len,now,home_row+i,home_line+j);
        ant_len++;
    }
    for(int i=-1,j=-1; j<3; j++){
        map[0][home_row+i][home_line+j] = '@';
        generate_new_ant(ant_len,now,home_row+i,home_line+j);
        ant_len++;
    }
    for(int i=4,j=-1; j<3; j++){
        map[0][home_row+i][home_line+j] = '@';
        generate_new_ant(ant_len,now,home_row+i,home_line+j);
        ant_len++;
    }
    
    
    for(int i=0;i<food_count;i++){
        food[ rand()%(row_side-2)+1 ][ rand()%(line_side-2)+1 ] += 1;
    }
    
    
    /*----------------------------------------------------------------------*/
    
    for(int tick=1; tick<=times; tick++){
    	
    	gotoxy(0,0);
    	Sleep(100);
    	
        int flag=0;
        printf("【第%d輪】\n",tick);
        home(home_row,home_line);
        if((tick%food_times) == 0){//每過指定次數生成一次食物
            for(int i=0;i<food_count;i++){
                int add=rand()%9+1;
                int x,y;
                while(true){
                    x=rand()%(row_side-2)+1;
                    y=rand()%(line_side-2)+1;
                    if((x<= home_row+2) && (x>= home_row) && (y<= home_line+2) && (y >= home_line)){
                        continue;
                    }
                    else if(food[x][y]==0){
                        break;
                    }
                }
                food[x][y] += add;
            }
        }
        struct ant *current = ant_top;
        for(int i=0;i<ant_len;i++){
            if(current->walk>0){
                if(current->food > 0){
                    int ge;
                    ge = backhome(current,home_row,home_line,ant_movetimes,now);
                    if(ge == true){
                        while(true){
                            int i=1;
                            if(map[next][home_row+i][home_line+1]!='@'){
                                generate_new_ant(ant_len,now,home_row,home_line+i);
                                break;
                            }
                            i++;
                        }
                        ant_len++;
                    }
                }
                else{
                    ant_move(current,ant_movetimes,now);
                }
            }
            if(current->walk==0){//死掉的要刪掉
                struct ant *del=ant_top;
                int a=0;
                while(del!=current&&del->next!=current){
                    //printf("%d %d\n",del->walk,jk);
                    del=del->next;
                    a++;
                }
                del->next=current->next;
                if(current==ant_top){
                    ant_top=current->next;
                    del=current->next;
                    flag=1;
                }
                struct ant *temp;
                temp=del;
                del=current;
                current=temp;
                free(del);
                ant_len--;
                /*
                if(del!=NULL){
                    printf("\n%d %d\n",del->row,del->line);
                    //free(del);
                }
                 */
            }
        }
        if(flag==0){
            current = current->next;//最後會變NULL
        }

        if(now == 0){
            now = 1;
            next=0;
        }
        else{
            now = 0;
            next=1;
        }
        for(int i=0;i<row_side+2;i++){
            for(int j=0;j<line_side+2;j++){
                if(map[now][i][j]!='@' && map[now][i][j]&&food[i][j]>0){
                    printf("%d",food[i][j]);
                }
                else{
                    printf("%c",map[now][i][j]);
                }
            }
            printf("\n");
        }
        for(int i=1;i<=row_side;i++){
            for(int j=1;j<=line_side;j++){
                hormone[i][j] -= 1;
            }
        }
    }
    return 0;
}


void home(int row,int line){
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++){
            map[0][row+i][line+j] = '#';
            map[1][row+i][line+j] = '#';
        }
    /*---------------------------------------*/
    
}
