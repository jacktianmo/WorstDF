#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
/*********************************************8***********任务序号从1开始**********************
********************************************************处理器的序号从1开始******************
*************
*/




const int task_number=4;													//任务数目
double processor_number=2;													//处理器数目
int ProcessorUsedNumber=0;													//已用的处理器的数目

struct Processors{

	double u;																//利用率
	int task_in[task_number];												//存入的是任务的序号。0表示无任务
	bool state;																//处理器是否使用 true是在用，false是没在用

};


/****************************************处理器初始化*****************************************/
void process_init_(Processors *processor)
{
	for(int i=0;i<processor_number;i++)													//5为处理器的数目;已解决
	{
		processor[i].state=false;
		processor[i].u=0;
		for(int j=0;j<task_number;j++)
		{
			processor[i].task_in[j]=0;
		}
	
	}

}

struct taskset{
		int c;
		int t;
		int d;
		double uTask;
		};

taskset task[task_number];															//定义了一个任务集
/****************************************产生任务集*******************************************/

void MakeTaskset()				//传入总的利用率
{
	task[2].c=task[0].c=4;
	task[1].c=task[3].c=6;
	task[2].t=task[0].t=10;
	task[2].d=task[0].d=10;
    task[1].t=task[3].t=14;
	task[1].d=task[3].d=14;
	task[2].uTask=task[0].uTask=(double)task[0].c/task[0].t;
	task[1].uTask=task[3].uTask=(double)task[1].c/task[1].t;
	  
	//test
	for(int i=0;i<4;i++)
		printf("生成任务%d：%d\n",i,task[i].c);
}




/*******************************************************sys-clock   类型问题（看任务集）******************************************/

/****************************************************单个任务求最小值(energy_min_freq)********************************************/
double energy_min_freq(const taskset *task,int n)
{//S=slack,I=idle duration

  int beta,deta;
  beta=deta=0;
  double alpha=1.0;
  bool IN_BZP=true;
  int Idle,S,w,w_pie,t;
  w=task[n].c;
  Idle=t=S=w_pie=0;
  do 
  {
	  if(IN_BZP==true)
	  {
		  do{
			 int temp=0;
		     for(int i=0;i<=n;i++)  //n的情况，数组从零开始
			 {
				temp+=task[i].c*((int)w/task[i].t+1);	//有问题
			}
			printf("在min_sys_clock%d\n",temp);
			w_pie=temp+S;
			deta=w_pie-w;
			w=w_pie;
			
	  }while(w<task[n].d && deta>0);
	  IN_BZP=false;	
	  }
	  else 
	  {
		  //求Idle 找一个最小值
		  int min=task[0].t*((int)w/task[0].t+1);
		  for(int j=0;j<=n;j++)
		  {	
			int temp=task[j].t*((int)w/task[j].t+1);
			if(temp<min)
			{
				min=temp;
				
					
			}//可以优化
		  }
	    Idle=min-w;
		S=S+Idle;
		w=w+Idle;
		t=w;
		beta=w-S;

        if((double)beta/t<alpha) alpha=(double)beta/t;
        IN_BZP=true; 
	 
	  }			
      


  }while(w<task[n].d);
  


  return alpha;
}

/*****************把任务按deadline递减排列(需验证)***********************************/
int Partition_task(taskset *task,int low,int high)
{
//	task[0]=task[low];
	taskset temp=task[low];
	double pivotkey=task[low].d;
	while(low<high){
		while(low<high && task[high].d>=pivotkey) --high;
		task[low]=task[high];
		while(low<high && task[low].d<=pivotkey)  ++low;
		task[low]=task[high];
	
	}
//	task[low]=task[0];
	task[low]=temp;
	return low;
}
void QSort_task(taskset *task,int low,int high)
{
	if(low<high){
	
		int pivotloc=Partition_task(task,low,high);
		QSort_task(task,low,pivotloc-1);
		QSort_task(task,pivotloc+1,high);
		
	}	
}
void QuickSort_task_deadline(taskset *task,int num)
{
	QSort_task(task,0,num-1);

}


/***************************************sys-clock 主体******************************************/
double sys_clock(taskset *task,int taskInNum)
{
	double temp[task_number];			//得到每个任务的临时变量

	
	/*****************把任务按deadline递减排列(未完成)***********************************/
	QuickSort_task_deadline(task,taskInNum);

	//求每个任务中的最小Fre
	for(int i=0;i<taskInNum;i++)
	{	
		
		temp[i]=energy_min_freq(task,i);
	
	}
	//找到所有任务中频率最高的
	for(int j=0;j<taskInNum;j++)
	{
		if(temp[j]>temp[j+1]) temp[j+1]=temp[j]; 
				
	}
	double ForReturn;
	ForReturn=temp[taskInNum-1];

	return ForReturn;

}

/*************************************wfd算法*****************************************************/



//找最小的利用率的处理器
int min_processor(const Processors *processor,int ProcessorUsedNumber)
{
	int temp=0;
	//找处理器的利用率的最小值

	for(int i=0;i<ProcessorUsedNumber;i++)
	{
		if(processor[i].u<processor[temp].u)
		{
			temp=i;
		}
	}
	
	printf("返回处理器的序号：%d\n",temp);
	//返回处理器的序号-1
	return temp;
}

/***********************************快速排序（未完成）***************************************/
int Partition(taskset *task,int low,int high)
{
	taskset temp=task[low];
	//task[0]=task[low];		有错
	double pivotkey=task[low].uTask;
	while(low<high){
		while(low<high && task[high].uTask>=pivotkey) --high;
		task[low]=task[high];
		while(low<high && task[low].uTask<=pivotkey)  ++low;
		task[low]=task[high];
	
	}
	task[low]=temp;
//	task[low]=task[0];    有错
	return low;
}
void QSort(taskset *task,int low,int high)
{
	if(low<high){
	
		int pivotloc=Partition(task,low,high);
		QSort(task,low,pivotloc-1);
		QSort(task,pivotloc+1,high);
		
	}	
}
void QuickSort(taskset *task)
{
	QSort(task,0,task_number-1);

}





/**********************************************WFD主体*****************************/
void WFD(taskset *task,Processors *processor)
{
	/*************对任务集按利用率快速排序****************/
	
	QuickSort(task);

	printf("排序完成\n");
	//test 快排有问题
	for(int j=0;j<4;j++)
		printf("任务：%d\n",task[j].c);

	int ProcessorUsedNumber=0;										//已用的处理器的数目，初始化为0


	/***********************主体**************************/ 
	//temp为最小的处理器的序号-1
	int temp;

	/*****************分派过程***************************/
	for(int i=0;i<task_number;i++)
	{   //task_u为任务的利用率	
		double task_u=task[i].uTask;
	   
		temp=min_processor(processor,ProcessorUsedNumber);			 //第2个参数需要想想
		printf("得到的处理器为%d\n",temp);
		if((processor[temp].u+task_u)<1 && processor[temp].state==true) {
			
			processor[temp].task_in[i]=(i+1);
			processor[temp].u=processor[temp].u+task_u;
			
		}
		else
		{
			
			processor[ProcessorUsedNumber].state=true;
			printf("前处理器使用状况：%d\n",ProcessorUsedNumber);
			processor[ProcessorUsedNumber].task_in[i]=(i+1);   //找一个为state为false的处理器的序号？   利用一个processorUsedNumber已解决
			processor[ProcessorUsedNumber].u=task_u;
			ProcessorUsedNumber+=1;
			printf("后处理器使用状况：%d\n",ProcessorUsedNumber);

		}
	
	}


}

/**********************处理每个处理器的任务集**********************************************/
  


void main()
{	
	MakeTaskset();
	Processors processor[2];
	process_init_(processor);
	//printf("处理器的状态：%d\n",processor[0].state);
	WFD(task,processor);



	double Fre[2];
	for(int i=0;i<2;i++)
	{	
		int taskFreNum=0;					//每次处理器从零开始计数
		taskset taskForFre[task_number];
		for(int j=0;j<4;j++)
		{
			printf("第%d个处理器分到的任务：%d\n",i,processor[i].task_in[j]);
			
			if(processor[i].task_in[j]!=0)
			{	
				printf("进入");
				taskset temp=task[processor[i].task_in[j]-1];
				taskForFre[taskFreNum]=temp;	
				printf("第%d个值为%d\n",i,taskForFre[taskFreNum].c);
				taskFreNum+=1;
			
			}
		  		

		}		   	
		
		Fre[i]=sys_clock(taskForFre,taskFreNum);
		printf("%d的频率为%lf",i,Fre[i]);
	/*test 为什么出错
		for(int x;x<taskFreNum;x++)
		{
			printf("%d,%d",i,taskForFre[x].c);
		}
*/

	

	}




}
