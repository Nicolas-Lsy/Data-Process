/*时深转换（程序模块1）**/
#include <stdio.h>
#define file1 "data.csv"
#define file2 "wave.txt"

int main(void)
{	
	FILE *fp1,*fp2,*fp3,*fp4,*fp5,*fp6,*fp;                         //井数据.xlsx文件，需另存为.csv文件
	double DEPTH,AC,DEN,V,T = 1712;                                 //深度（DEPTH）,声波时差（AC）,密度（DEN）,T的初始值
	double DEPTH_1 = 50.01,DEPTH_2,temp_1,temp_2,V_last;            //DEPTH_1初始值 ,这里的初值文件不一样 需要更改 所以需要注意
	char line[50];                                                  //读取每行csv数据
	int count = 1,temp = 0;
	char file[20];	
	char Y_N;
	int a,b,max,flag = 1711;
	double DEPTH_three, T_result_3, V_3, DEN_3,DEPTH_two, T_result_2, V_2, DEN_2, DEPTH_one, T_result_1, V_1, DEN_1;
	int i=1,L=25,N=0,n=1;                                                    // L子波个数，n采样序号
	double num_Z[500], num_T[500],num_R[500],num_b[500],S = 0;


	
	
	printf("Your files are 'data.csv' and 'wave.t' (Y/N) ? \n");
	scanf("%c", &Y_N);
	if(Y_N=='n'||Y_N=='N'){
		printf("You can change them from program ...\n");
		getchar();
		return 0;
	}
	
	
/*************从数据中得到V************************************/
	fp1 = fopen(file1, "r");  	                               //打开"原始数据"文件
	
		if(fp1==NULL) 
			printf("\n不能打开 井数据 这个文件");
	
	fp2 = fopen("new.csv", "w");	                               //打开"V"文件(新建)
	
		if(fp2==NULL) 
			printf("\n不能打开 new.csv 这个文件");
	
	V_last = 1000000/186.2993;                                    //上一个V的值
	while(fgets(line, 50, fp1)){
		
		sscanf(line, "%lf,%lf,%lf\n", &DEPTH,&AC,&DEN);           //获取数据

		V = 1000000/AC;  	                                      //求得V值
		
		DEPTH_2 = DEPTH;
		T = T + 2000*(DEPTH_2-DEPTH_1)/V_last;                    //求得T值
	
	  //printf("#%d  DEPTH = %lf,AC = %lf,DEN = %lf, V = %lf, T = %lf\n", count,DEPTH,AC,DEN,V,T);
	  //count++;                                                        
	
		fprintf(fp2, "%lf,%lf,%lf,%lf \n",DEPTH,T,V,DEN);          //输入
		V_last = V;
		DEPTH_1 = DEPTH_2;
  } 
    fclose(fp1);
	fclose(fp2);
		
/**************将T进行筛选**********************************/
	
	fp2 = fopen("new.csv", "r");                                   //打开new数据文件
	if(fp2==NULL) printf("\n不能打开 new 这个文件");
	
	
	fp4 = fopen("new_T.csv", "w");                                 //筛选后数据文件
	if(fp4==NULL) printf("\n不能打开 new_T.csv 这个文件");
		
   	
	
	count = 1712;                                                  //一些临时变量

	
	while(fgets(line, 50, fp2)){                                   //循环筛选相似数据		 
		sscanf(line, "%lf,%lf,%lf,%lf\n", &DEPTH,&T,&V,&DEN);      //获取每行数据
		if((int)(T)==count)                                        //'上面'最可能接近值  
			{
				if((int)(T*10000)%10000==0){
					
					fprintf(fp4, "%lf,%lf,%lf,%lf\n",DEPTH,T,V,DEN);//获取第一行数据
				}                                                    
			
				max = 0;
				if(max <= (int)((T - count)*100)){                 //当前点最大，即下一点的最可能接近值
					max = (int)((T-count)*100);
					T_result_1 = T;                                //截取T
					DEPTH_one = DEPTH;                             //截取DEPTH
					V_1 = V;									   //截取V
					DEN_1 = DEN;                                   //截取DEN
			    }
           }
		else
			{                                                      //下一点最小
				T_result_3 = T;                                
				DEPTH_three = DEPTH;                             
				V_3 = V;									   
				DEN_3 = DEN; 
				count++;
			
		a = (int)((count - T_result_1)*1000000);                  //相似值1
		b = (int)((T_result_3 - count)*1000000);	              //相似值2
		
		if(a>=b)                                                  //选择最相似为new_T.csv
			fprintf(fp4, "%lf,%lf,%lf,%lf \n",DEPTH_three,T_result_3,V_3,DEN_3);
		else
			fprintf(fp4, "%lf,%lf,%lf,%lf \n",DEPTH_one,T_result_1,V_1,DEN_1);
			}
			
 }
    fclose(fp2);
	fclose(fp4);

/************************再次隔二筛选****************************************/

	fp3 = fopen("new_T_2.csv", "w"); 
	if(fp3==NULL) printf("\n不能打开 new_T_2.csv 这个文件");	  //T二次筛选后数据文件
	
	fp4 = fopen("new_T.csv", "r");                                //T初次筛选后数据文件
	if(fp4==NULL) printf("\n不能打开 new_T.csv 这个文件");        
	
	while(fgets(line, 50, fp4)){                                  //循环提出不符合的
		
		flag++;
		sscanf(line, "%lf,%lf,%lf,%lf\n", &DEPTH,&T,&V,&DEN);
		if(flag%2==0){
			fprintf(fp3, "%lf,%d,%lf,%lf \n",DEPTH,flag,V,DEN);

		}
		
	}
	fclose(fp3);
	fclose(fp4);




/**************************求得Z和R的值并且输出********************************************/


/*合成地震记录（程序模块2）*/




fp = fopen("Z.txt", "w"); 
fp3 = fopen("new_T_2.csv", "r"); 
	while(fgets(line,500,fp3))                                          //读取new_T_2.csv文件
	{
		sscanf(line,"%lf,%lf,%lf,%lf", &DEPTH,&T,&V,&DEN);
		num_Z[i] = V * DEN;                                              //求得Z值
		printf("Test Z[%d] = %lf\n", i,num_Z[i]);                       //测试代码 可删除
		fprintf(fp,"Z[%d] = %lf\n", i,num_Z[i]); 
		num_T[i] = T;                                                   //对于T值
		i++;
		N = i-1;                                                        //Z的个数N  114
		count = N;
	}
	fclose(fp3);
	fclose(fp);
	
	
fp = fopen("R.txt", "w"); 
for(i = 1; i<N; i++){
	num_R[i] = (num_Z[i+1]-num_Z[i])/(num_Z[i+1]+num_Z[i]);
	printf("Test R[%d] = %lf\n", i,num_R[i]);                       	//测试代码 可删除
	fprintf(fp," R[%d] = %lf\n", i,num_R[i]);
	}                                                                   //求取R,个数N 113
	N = N - 1;                                                         //R的个数 比Z少1
	fclose(fp);



/*****************************得到子波数据******************************/
fp5=fopen(file2,"r");                                                 //获取 子波
if(fp5==NULL) printf("\n不能打开 子波 这个文件");

    for(i=1;i<=L;i++){
		fscanf(fp5,"%lf",&num_b[i]);
	    printf("Test b[%d] = %lf\n", i,num_b[i]);
	}
	fclose(fp5);
/**********************************************************************/

fp6 = fopen("new_S.txt", "w");                                     //写入S记录
	for(n=1; n<=L+N; n++){
	
		for(i=1; i<=L; i++){                                      //L = 25
		
			if((n-i>0) && ((n-i) < N))
			S +=  (num_R[n - i]) * (num_b[i]); 
		
    
		}                                                      //叠加从S[1]到S[25]
		                                                          //S(n)=
		//if(n>N)
			//S = 0;
		
		printf("S[%d] = %lf\n",n,S);
		fprintf(fp6,"S[%d] = %lf\n",n,S);
		S = 0;

}
	fclose(fp6);

	return 0;
}

/******************************************
1.功能已经全部实现 
2.代码排版缩进均有些凌乱，如需要 后续可修改。
3.文件excel用C语言无法读取，需另存为csv格式，内容不会改变的
4.存在测试代码，需要后续删除
5.对应文件名可以自行修改  
*******************************************/
