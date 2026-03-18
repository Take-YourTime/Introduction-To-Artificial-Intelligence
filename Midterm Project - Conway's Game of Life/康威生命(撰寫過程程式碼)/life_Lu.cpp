#include <stdio.h> 
#include<time.h> 
#include<stdlib.h>
#define ROW 10 
#define COLUMN 10



int main() 
{
	int f[ROW][COLUMN],before[ROW][COLUMN];
           
    srand(time(NULL));
    
    int i,j;
	
	for(i=0;i<ROW;i++)
				{
					for(j=0;j<COLUMN;j++)
						{
							f[i][j]=rand()%2;
							before[i][j]=f[i][j];
						}
				}
	
	
	
	
	
	
	int sum=0,g[ROW][COLUMN],z=1,x=1;
	
	
	while(1)
		{
			
			x*=-1;	
			
			
			for(i=0;i<ROW;i++)
				{
					for(j=0;j<COLUMN;j++)
						{
							if(f[i][j]==1)
								printf("*");
							else if(f[i][j]==0) 
								printf("~");
						}
					printf("\n");
				}
						
			printf("status\n");
			
			for(i=0;i<ROW;i++)
				{
					for(j=0;j<COLUMN;j++)
						{
							if((i-1)>=0 && (j-1)>=0)
								sum+=f[i-1][j-1];
							if((i-1)>=0)
								sum+=f[i-1][j];
							if((i-1)>=0 && (j+1)<=9)
								sum+=f[i-1][j+1];
							if((j+1)<=9)
								sum+=f[i][j+1];
							if((i+1)<=9 && (j+1)<=9)
								sum+=f[i+1][j+1];
							if((i+1)<=9)
								sum+=f[i+1][j];
							if((i+1)<=9 && (j-1)>=0)
								sum+=f[i+1][j-1];
							if((j-1)>=0)
								sum+=f[i][j-1];
							
							g[i][j]=sum;
							//printf("%d\n",sum);
							sum=0;
						}
			
				}
			
			
			for(i=0;i<ROW;i++)
				{
					for(j=0;j<COLUMN;j++)
						{
							if(f[i][j]==1 && g[i][j]<2)
								f[i][j]=0;
							else if(f[i][j]==1 && g[i][j]==2 || g[i][j]==3)
								f[i][j]=1;
							else if(f[i][j]==1 && g[i][j]>3)
								f[i][j]=0;
							else if(f[i][j]==0 && g[i][j]==3)
								f[i][j]=1;
						}
				}
		
		
			for(i=0;i<ROW;i++)
				{
					for(j=0;j<COLUMN;j++)
						{
							if(before[i][j]!=f[i][j])
								{
									z*=-1;
									break;
								}
						}
					if(z*x==1)
						break;
					
				}
			
			
			if((z*x)==-1)
				break;
			
			
			
			for(i=0;i<ROW;i++)
				{
					for(j=0;j<COLUMN;j++)
						{
							before[i][j]=f[i][j];
						}
					
				}
		
		
		
		}
	

	
	return 0;
}
