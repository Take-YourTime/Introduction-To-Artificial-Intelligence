#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<ctype.h>
#include<limits.h>
#include<windows.h>

#define true 1
#define false 0

#define row_side 40
#define line_side 187
#define walk_times 100
#define det_hormone 10
#define sec 0

char map[2][row_side + 2][line_side + 2];    //1~side（+2用於設置地圖邊界）
//0、side+1為邊界
int hormone[row_side + 2][line_side + 2];
int food[row_side + 2][line_side + 2];
double dis[row_side + 2][line_side + 2];


struct node {
    int row;
    int line;
    struct node* ptr;
};

struct node* top = NULL;


struct ant {
    int row;
    int line;

    int desrow;    //行動目的地
    int desline;

    int food;    //記錄身上是否有食物
    int walk;    //可以走幾步(生命值)
    int job;    //螞蟻的工作
    int job4_times;
    struct ant* next;    //紀錄下一隻螞蟻，原本是想用array儲存，但問題是要增加的時候用array會不好做
    struct ant* pre;    //紀錄上一隻螞蟻
};
/*
【 job list 】
    1.find food
    2.go back to home
    3.follow hormine
*/

struct ant* rear = NULL;
struct ant* front = NULL;


/*-------------------------------------*/
void gotoxy(int x, int y) {//刷新頁面
    COORD pos = { x,y };
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}


void Show_Cursor(int visible) { //隱藏光標
    CONSOLE_CURSOR_INFO cursor_info = {20, visible};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
/*-------------------------------------*/
void home(int row, int line,int);
void output(int);
void swap(int* a, int* b);
int search(int row, int line);

void generate_new_ant(int row, int line);
void enqueue(int row, int line);
void dequeue(struct ant* dead);

void push(int row, int line);
void pop(void);

void ant_detect(struct ant *, int);
void ant_normalmove(struct ant *);
int distance(int, int, int, int);
void farthest(struct ant *);
/*-------------------------------------*/


int now = 0, next = 1;    //切換矩陣
int home_row, home_line;    //蟻巢中心點
int ant_len = 0;    //有幾隻螞蟻

int main() {
/*地圖歸零----------------------------------------------------------------------*/
    for (int i = 0; i < row_side + 2; i++) {
        for (int j = 0; j < line_side + 2; j++) {
            map[now][i][j] = ' ';
            map[next][i][j] = ' ';
            hormone[i][j] = 0;
            food[i][j] = 0;
        }
    }
    
    for (int i = 0, j = 0; j < line_side + 2; j++) {				//上邊界
        map[0][i][j] = '-';
        map[1][i][j] = '-';
    }
    for (int i = row_side + 1, j = 0; j < line_side + 2; j++) {		//下邊界
        map[0][i][j] = '-';
        map[1][i][j] = '-';
    }
    for (int i = 0, j = 0; i < row_side + 2; i++) {					//左邊界
        map[0][i][j] = '|';
        map[1][i][j] = '|';
    }
    for (int i = 0, j = line_side + 1; i < row_side + 2; i++) {		//右邊界
        map[0][i][j] = '|';
        map[1][i][j] = '|';
    }
/*----------------------------------------------------------------------*/

/*設置蟻巢(3x3)*/
    srand(time(NULL));
    home_row = rand() % (row_side - 8) + 4;    //side-3以避免蟻巢超出地圖邊界
    home_line = rand() % (line_side - 8) + 4;
    home(home_row, home_line,now);
    home(home_row, home_line,next);
/*-------------*/

    for (int i = 1; i <= row_side; i++) {		//地圖上每個點與家的距離
        for (int j = 1; j <= line_side; j++) {
            int x, y;
            double ans;
            x = i - home_row;
            y = j - home_line;

            ans = sqrt((x * x + y * y));

            dis[i][j] = ans;
        }
    }

    /*----------------------------------------------------------------------*/

    int ant_movetimes = 1, food_count, food_produce_cycle,food_decrease_cycle;
    int tick = 1;       //第幾代

    printf("請問一次要生成幾個食物？ ");
    scanf("%d", &food_count);
    printf("請問多久要生成一次食物？ ");
    scanf("%d", &food_produce_cycle);
    printf("請問食物衰減的週期為？ ");
    scanf("%d", &food_decrease_cycle);
    
	Show_Cursor(false);
    /*-------------*/
    //設置初始螞蟻
    for (int i = -2, j = -2; i < 3; i++) {        		//左
        generate_new_ant(home_row + i, home_line + j);
    }
    for (int i = -2, j = 2; i < 3; i++) {        		//右
        generate_new_ant(home_row + i, home_line + j);
    }
    for (int i = -2, j = (-2) + 1; j < 3 - 1; j++) {    //上
        generate_new_ant(home_row + i, home_line + j);
    }
    for (int i = 2, j = (-2) + 1; j < 3 - 1; j++) {    //下
        generate_new_ant(home_row + i, home_line + j);
    }
    /*-------------*/

    for (int i = 0; i < food_count; i++) {    //產生初始食物
        int x, y;

        while (true) {    //食物不可生在蟻巢
            x = rand() % (row_side)+1;
            y = rand() % (line_side)+1;

            if (map[now][x][y] == ' ')    //食物需生在空的位置
                break;
        }

        food[x][y] = rand() % 6 + 1;    //食物數量：1~5
        map[now][x][y] = '0' + food[x][y];
    }

    /*----------------------------------------------------------------------*/
    system("cls");    //清除螢幕
    printf("【第%d輪】  螞蟻數量: %d隻\n",tick,ant_len);
    output(now);    //print original matric
    tick++;

    while (true) {
        gotoxy(0,0);	//從頭印出
        Sleep(sec);
        //system("pause");
        printf("【第%d輪】  螞蟻數量: %d隻\n",tick,ant_len);
        tick++;
        
        if((tick%food_decrease_cycle) == 0){		//每過指定次數生成一次食物
        	for(int i=1; i<=row_side;i++){	//將所有已存在的食物數量-1
                for(int j=1; j<=line_side; j++){
                    if(food[i][j]>1){
                        food[i][j]--;
                        map[now][i][j] = food[i][j] + '0';
                    }
                    else if(food[i][j] == 1){
                        food[i][j] = 0;
                        map[now][i][j] = ' ';
                        map[next][i][j] = ' ';
					}
                }
            }
		}
        
        
        if ((tick % food_produce_cycle) == 0) {
            for (int i = 0; i < food_count; i++) {	//生成新的食物
                int x, y;
                while (true) {
                    x = rand() % (row_side)+1;
                    y = rand() % (line_side)+1;

                    if (map[next][x][y] == ' ')    //食物需生在空的位置
                        break;
                }

                food[x][y] = rand() % 6 + 1;    //食物數量：1~5
                map[next][x][y] = '0' + food[x][y];
            }
        }

        struct ant* current = front;
        while (current != NULL) {    //走訪每隻螞蟻
            (current->walk)--;
            if ((current->walk) == 0) {    //螞蟻死亡
                struct ant* temp = current;
                current = temp->pre;
                dequeue(temp);
                ant_len--;
                continue;
            }
            else if((current->desrow) == (current->row) && (current->desline) == (current->line)){	//抵達目的地後，設置新的目的地
            	do {
        			current->desrow = rand()%row_side+1;
        			current->desline = rand()%line_side+1;
    			} while (map[now][current->desrow][current->desline] == '#');
			}

            ant_detect(current, ant_movetimes);

            current = current->pre;
        }
        
        /*輸出矩陣*/
        for(int i=1; i<=row_side;i++){	//將食物複製到下一個矩陣
            for(int j=1; j<=line_side; j++){
            	if(food[i][j] > 0)
                    map[next][i][j] = food[i][j]+'0';
            }
        }
        
        swap(&now, &next);
        home(home_row,home_line,now);
        output(now);			//print matric
        /*------------*/
        if (ant_len == 0) {		//所有螞蟻死亡
            goto end;
        }
    }

    
end:
    printf("【Program ending】\n");
    
    return 0;
}


void home(int row, int line,int list) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            map[list][row + i][line + j] = '#';
        }
    }
}


void output(int list) {
    for (int i = 0; i < row_side + 2; i++) {
        for (int j = 0; j < line_side + 2; j++)
            printf("%c", map[list][i][j]);
        printf("\n");
    }
}


void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


int search(int row, int line) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (food[row][line] > 0)
                return true;
        }
    }

    return false;
}


void generate_new_ant(int row, int line) {
    map[now][row][line] = '@';
    enqueue(row, line);
    ant_len++;
}


void enqueue(int row, int line) {
    struct ant* current = malloc(sizeof(struct ant));

    current->row = row;
    current->line = line;
    current->walk = walk_times;
    current->job = 1;    //初始設定螞蟻的工作為尋找食物
    current->job4_times = 0;

    /*設定螞蟻行動目的地*/
    do {
        current->desrow = rand()%row_side+1;
        current->desline = rand()%line_side+1;
    } while (map[now][current->desrow][current->desline] == '#');

    if (rear == NULL) {    //rear = front = NULL
        current->next = front;
        current->pre = rear;
        rear = current;
        front = current;
    }
    else {
        rear->pre = current;
        current->next = rear;
        current->pre = NULL;

        rear = current;
    }
}


void dequeue(struct ant* dead) {
    
    map[now][dead->row][dead->line] = ' ';
    
    if (dead == rear && dead == front) {
        rear = NULL;
        front = NULL;
        free(dead);
    }
    else if (dead == rear) {
        rear = dead->next;
        dead->next->pre = NULL;
        free(dead);
    }
    else if (dead == front) {
        front = dead->pre;
        dead->pre->next = NULL;
        free(dead);
    }
    else {
        dead->pre->next = dead->next;
        dead->next->pre = dead->pre;
        free(dead);
    }
}


void ant_detect(struct ant* current, int ant_movetimes) {    //偵測螞蟻要做甚麼工作
    int row = current->row, line = current->line;
    int job = current->job;
    int max_row = -1, max_line = -1;

    int hormone_value;    //九宮格中超過偵測值的最小賀爾蒙值
    int deal_hormone;

    int max_food;
    int len = 0;    //周圍格子中的食物為max_food的數量


    for (int count = 0; count < ant_movetimes; count++) {
        hormone_value = 0;
        max_food = 0;
        deal_hormone = false;
        int row_hormone;
        int line_hormone;
        
        switch (job) {
        case 1:;    //find food

            for (int i = -1; i <= 1; i++) {    //掃周圍的九宮格(螞蟻的視野可見周圍九宮格有無食物)
                for (int j = -1; j <= 1; j++) {
                    if ((hormone[row + i][line + i]) > det_hormone) {    //探測賀爾蒙值是否大於det_hormone
                        row_hormone = row + i;
                        line_hormone = line + j;
                        deal_hormone = true;
                    }

                    if (!isdigit(map[next][row + i][line + i]))    //該格子不是食物
                        continue;

                    /*尋找九宮格中食物最多的格子*/
                    if (food[row + i][line + j] > max_food) {
                        max_food = food[row + i][line + j];
                        max_row = row + i;
                        max_line = line + j;

                        while (top != NULL) {    //當找到的格子中有更多的食物時，清空stack，並將該格存入stack中
                            pop();
                        }
                        push(max_row, max_line);
                        len = 1;
                    }
                    else if (food[row + i][line + j] == max_food) {    //食物數量相同時，存入stack
                        push(row + i, line + j);
                        len++;
                    }
                }
            }

            if (max_food != 0) {	//周圍有食物
                if (len > 1) {		//有相同數量的食物時，隨機以其中一個格子作為目的地
                    struct node* temp = top;
                    int decide = rand() % len;

                    for (int i = 0; i < decide; i++) {
                        temp = temp->ptr;
                    }
                    max_row = temp->row;
                    max_line = temp->line;
                }

                /*移動螞蟻到食物地點*/
                map[now][current->row][current->line] = ' ';
                current->row = max_row;
                current->line = max_line;
                current->job = 2;    //切換到go back to home
                map[next][current->row][current->line] = '@';
				
				if(food[max_row][max_line] == 1){	//食物數量 = 1時，將當前矩陣的格子改成' '
					food[max_row][max_line] = 0;
					map[now][max_row][max_line] = ' ';
				}
				else{
					food[max_row][max_line]--;    //食物數量-1
				}
            }
            else {    //max_food = 0
                /*是否依循賀爾蒙？*/
                if (deal_hormone == true) {
                    /*移動螞蟻到賀爾蒙處*/
                    map[now][current->row][current->line] = ' ';
                    current->row = row_hormone;
                    current->line = line_hormone;

                    if (search(current->row, current->line) == false)
                        current->job = 3;    //周圍沒有食物，切換到follow hormone

                    map[next][current->row][current->line] = '@';
                }
                else{    //正常移動螞蟻
                    ant_normalmove(current);
                }
            }

            while (top != NULL)    //清空stack
                pop();

            break;


        case 2:;    //go back to home
            struct list {
                int row;
                int line;
                double distance;
            }way[3];

            /* 順序：由右往左，由上往下
            0 1 2
            3   4
            5 6 7
            */

            if (row > home_row) {
                if (line > home_line) {        //螞蟻在家的右下
                    way[0].row = row - 1;
                    way[0].line = line - 1;
                    way[0].distance = distance(home_row, home_line, row - 1, line - 1);

                    way[1].row = row - 1;
                    way[1].line = line;
                    way[1].distance = distance(home_row, home_line, row - 1, line);

                    way[2].row = row;
                    way[2].line = line - 1;
                    way[2].distance = distance(home_row, home_line, row, line - 1);
                }
                else if (line < home_line) {    //螞蟻在家的左下
                    way[0].row = row - 1;
                    way[0].line = line;
                    way[0].distance = distance(home_row, home_line, row - 1, line);

                    way[1].row = row - 1;
                    way[1].line = line + 1;
                    way[1].distance = distance(home_row, home_line, row - 1, line + 1);

                    way[2].row = row;
                    way[2].line = line + 1;
                    way[2].distance = distance(home_row, home_line, row, line + 1);
                }
                else {                        //螞蟻在家的正下方
                    way[0].row = row - 1;
                    way[0].line = line - 1;
                    way[0].distance = distance(home_row, home_line, row - 1, line - 1);

                    way[1].row = row - 1;
                    way[1].line = line;
                    way[1].distance = distance(home_row, home_line, row - 1, line);

                    way[2].row = row - 1;
                    way[2].line = line + 1;
                    way[2].distance = distance(home_row, home_line, row - 1, line + 1);
                }
            }
            else if (row < home_row) {
                if (line > home_line) {        //螞蟻在家的右上
                    way[0].row = row;
                    way[0].line = line - 1;
                    way[0].distance = distance(home_row, home_line, row, line - 1);

                    way[1].row = row + 1;
                    way[1].line = line - 1;
                    way[1].distance = distance(home_row, home_line, row + 1, line - 1);

                    way[2].row = row + 1;
                    way[2].line = line;
                    way[2].distance = distance(home_row, home_line, row + 1, line);
                }
                else if (line < home_line) {    //螞蟻在家的左上
                    way[0].row = row;
                    way[0].line = line + 1;
                    way[0].distance = distance(home_row, home_line, row, line + 1);

                    way[1].row = row + 1;
                    way[1].line = line;
                    way[1].distance = distance(home_row, home_line, row + 1, line);

                    way[2].row = row + 1;
                    way[2].line = line + 1;
                    way[2].distance = distance(home_row, home_line, row + 1, line + 1);
                }
                else {                        //螞蟻在家的正上方
                    way[0].row = row + 1;
                    way[0].line = line - 1;
                    way[0].distance = distance(home_row, home_line, row + 1, line - 1);

                    way[1].row = row + 1;
                    way[1].line = line;
                    way[1].distance = distance(home_row, home_line, row + 1, line);

                    way[2].row = row + 1;
                    way[2].line = line + 1;
                    way[2].distance = distance(home_row, home_line, row + 1, line + 1);
                }
            }
            else {    //home_row = row
                if (line > home_line) {        //螞蟻在家的正右方
                    way[0].row = row + 1;
                    way[0].line = line - 1;
                    way[0].distance = distance(home_row, home_line, row + 1, line - 1);

                    way[1].row = row;
                    way[1].line = line - 1;
                    way[1].distance = distance(home_row, home_line, row, line - 1);

                    way[2].row = row - 1;
                    way[2].line = line - 1;
                    way[2].distance = distance(home_row, home_line, row - 1, line - 1);
                }
                else {    //homeline < line    //螞蟻在家的正左方
                    way[0].row = row + 1;
                    way[0].line = line + 1;
                    way[0].distance = distance(home_row, home_line, row + 1, line + 1);

                    way[1].row = row;
                    way[1].line = line + 1;
                    way[1].distance = distance(home_row, home_line, row, line + 1);

                    way[2].row = row - 1;
                    way[2].line = line + 1;
                    way[2].distance = distance(home_row, home_line, row - 1, line + 1);
                }
            }


            int min_distance = INT_MAX, det;    //det暫存 i 的值

            for (int j = 0; j < 3; j++) {    //找到離家最近的點
                if (way[j].distance < min_distance) {
                    min_distance = way[j].distance;
                    det = j;
                }
            }
            map[now][current->row][current->line] = ' ';
            current->row = way[det].row;
            current->line = way[det].line;
            map[next][way[det].row][way[det].line] = '@';
            
            if (abs(row - home_row) <= 2) {	//判斷是否抵達家中
                if (abs(line - home_line) <= 2) {    //抵達家的四週
                    current->job = 1;
                    current->walk = (current->walk)*1.3 + (walk_times*(1/5));
                    do {                            //重新設定目的地
                        current->desrow = rand() % (row_side)+1;
                        current->desline = rand() % (line_side)+1;
                    } while (map[now][current->desrow][current->desline] == '#');
                    
                    /*產生新的螞蟻*/
                    int x=-2,y=-2;	//range: -2 ~ 2
                    /*
					x = rand()%5-2;
                    y = rand()%5-2;
                    */
                    generate_new_ant((home_row+x),(home_line+y));
                }
            }

            break;

        case 3:    //follow hormone
            farthest(current);	//go to farthest
            for (int i = -1; i <= 1; i++) {    //掃周圍的九宮格(螞蟻的視野可見周圍九宮格有無食物)
                for (int j = -1; j <= 1; j++) {
                    if ((hormone[row + i][line + i]) > det_hormone) {    //探測賀爾蒙值是否大於det_hormone
                        row_hormone = row + i;
                        line_hormone = line + j;
                        deal_hormone = true;
                    }

                    if (!isdigit(map[next][row + i][line + i]))    //該格子不是食物
                        continue;

                    /*尋找九宮格中食物最多的格子*/
                    if (food[row + i][line + j] > max_food) {
                        max_food = food[row + i][line + j];
                        max_row = row + i;
                        max_line = line + j;

                        while (top != NULL) {    //當找到的格子中有更多的食物時，清空stack，並將該格存入stack中
                            pop();
                        }
                        push(max_row, max_line);
                        len = 1;
                    }
                    else if (food[row + i][line + j] == max_food) {    //食物數量相同時，存入stack
                        push(row + i, line + j);
                        len++;
                    }
                }
            }

            if (max_food != 0) {
                current->job=1;
            }
            else {    //max_food = 0
                if (deal_hormone == true) {
                    int hormone_dis,me_dis;
                    hormone_dis = distance(row_hormone,line_hormone,home_row,home_line);
                    me_dis = distance(row,line,home_row,home_line);
                        
                    if(me_dis > hormone_dis){
                        current->job=4;
                        //下一個強烈賀爾蒙的位置在離家比較近的位置，代表會回頭，所以要切換工作到job4
                    }
                    else{
                        current->job=3;
                        //繼續job3
                    }
                }
                else{
                    current->job=1;
                    //假如沒有找到強烈的賀爾蒙就回到job1
                }
            }
            while (top != NULL)    //清空stack
                pop();
                
            break;
            
        case 4:
            current->job4_times += 1;
            for (int i = -1; i <= 1; i++) {    //掃周圍的九宮格(螞蟻的視野可見周圍九宮格有無食物)
                for (int j = -1; j <= 1; j++) {
                    if(current->job4_times==5){		//檢查賀爾蒙，但只有第五次的時候會用到
                    	if ((hormone[row + i][line + i]) > det_hormone) {    //探測賀爾蒙值是否大於det_hormone
                        	row_hormone = row + i;
                        	line_hormone = line + j;
                        	deal_hormone = true;
                    	}
                	}
                    
                    if (!isdigit(map[next][row + i][line + i]))    //該格子不是食物
                        continue;

                    /*尋找九宮格中食物最多的格子*/
                    if (food[row + i][line + j] > max_food) {
                        max_food = food[row + i][line + j];
                        max_row = row + i;
                        max_line = line + j;

                        while (top != NULL) {    //當找到的格子中有更多的食物時，清空stack，並將該格存入stack中
                            pop();
                        }
                        push(max_row, max_line);
                        len = 1;
                    }
                    else if (food[row + i][line + j] == max_food) {    //食物數量相同時，存入stack
                        push(row + i, line + j);
                        len++;
                    }
                }
            }

            if (max_food != 0) {
                if (len > 1) {    //有相同數量的食物時，隨機以其中一個格子作為目的地
                    struct node* temp = top;
                    int decide = rand() % len;

                    for (int i = 0; i < decide; i++) {
                        temp = temp->ptr;
                    }
                    max_row = temp->row;
                    max_line = temp->line;
                }

                /*移動螞蟻到食物地點*/
                map[now][current->row][current->line] = ' ';
                current->row = max_row;
                current->line = max_line;
                current->job = 2;    //切換到go back to home
                map[next][current->row][current->line] = '@';

                if(food[max_row][max_line] == 1){	//食物數量 = 1時，將當前矩陣的格子改成' '
					food[max_row][max_line] == 0;
					map[next][max_row][max_line] = ' ';
				}
				else{
					food[max_row][max_line]--;    //食物數量-1
				}
            }
            else {    //max_food = 0
                //沒有食物就正常走
                ant_normalmove(current);
            }

            while (top != NULL)    //清空stack
                pop();
                
                
            if(current->job4_times == 5){
                current->job4_times = 0;
                if(deal_hormone){
                    current->job=3;
                }
                else{
                    current->job=1;
                }
            }
                
        	break;
        	
        default:
            break;
        }
    }
}


void push(int row, int line) {
    struct node* temp = malloc(sizeof(struct node));

    temp->row = row;
    temp->line = line;

    if (top == NULL) {
        temp->ptr = NULL;
        top = temp;
    }
    else {
        temp->ptr = top;
        top = temp;
    }
}


void pop(void) {
    if (top != NULL) {
        struct node* temp = top;
        top = temp->ptr;
        free(temp);
    }
}


void ant_normalmove(struct ant *current) {
    int desrow = current->desrow;
    int desline = current->desline;
    int row = current->row;
    int line = current->line;
    
    int lenth[3][3];
    int min_row, min_line;

    for (int i = -1; i <= 1; i++) {    //測量九宮格每格與目的地的距離
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;

            lenth[i + 1][j + 1] = distance(desrow, desline, row+i, line+j);
        }
    }

     
    int min_lenth = INT_MAX;
    min_row = 0, min_line = 0;

    for (int i = -1; i <= 1; i++) {        //找最小距離
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;

            if (lenth[i + 1][j + 1] < min_lenth) {
                min_lenth = lenth[i + 1][j + 1];
                min_row = row + i;
                min_line = line + j;
            }
        }
    }
    
    map[now][row][line] = ' ';
    map[next][min_row][min_line] = '@';
    current->row = min_row;
    current->line = min_line;
}


int distance(int A, int B, int a, int b) {
    return ((A - a) * (A - a) + (B - b) * (B - b));
}


void farthest(struct ant *current){		//go to farthest
    int len[8][3];	//每格的長度
    int k = 0;
    int row = current->row;
    int line = current->line;
    
    for(int i=-1; i<=1; i++){
        for(int j=-1; j<=1; j++){
            if(i==0 && j==0){
                continue;
            }
            
            len[k][0] = distance(row+i, line+j, home_row, home_line);
            len[k][1] = row+i;
            len[k][2] = line+j;
			line+j;
            k++;
        }
    }
    for(int i=0; i<7; i++){		//排序找最遠
        for(int j=0; j<7-1-i; j++){
            if(len[j][0] < len[j+1][0]){
                swap( &len[j][0],&len[j+1][0] );
                swap( &len[j][1],&len[j+1][1] );
                swap( &len[j][2],&len[j+1][2] );
            }
        }
    }
    map[now][row][line] = ' ';
    map[next][len[0][1]][len[0][2]] = '@';
    current->row = len[0][1];
    current->line = len[0][2];
}