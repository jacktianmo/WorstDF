#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
/*********************************************8***********任务序号从1开始**********************
********************************************************处理器的序号从1开始******************
*************
*/




const int task_number=25;													//任务数目
double processor_number=5;													//处理器数目
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
void MakeTaskset(double total_ui)				//传入总的利用率
{
	
		if(total_ui>1) printf("错误的任务集");
		int i=0;
		double max_ui=0.5;															//任务集中的任务的最大的利用率
		double temp_ui=0;															//利用率的临时变量，记录总的利用率
		while(i<task_number)
		{
			/*
			s=0.000;
			smax=0.00000;
			si=1.0000;
		    for
			*/	
			

			/****初始化周期t*******/
			
			int temp_t;
			temp_t=rand()%3;
			switch (temp_t){
			case 0:task[i].t=1+rand()%10;break;        //周期从1到10
			case 1:task[i].t=11+rand()%90;break;       //周期从11到100
			case 2:task[i].t=101+rand()%900;break;     //周期从101到1000
			}


			/**********初始化运行时间c**********/
			while(task[i].c<=(task[i].t*0.5))     
			{
				
			}
			
			/********初始化deadline d************/
			/*这里令周期与deadline相等*/
			task[i].d=task[i].d;

			/*计算出利用率uTask*/
			//	task[i].uTask=task[i].c/task[i].t; 修改
			temp_ui+=task[i].uTask;
			




				
				
		}
		/*判断temp_ui与total_ui相等*/
		if(temp_ui==total_ui)
			printf("生成任务成功");
		else
			printf("生成任务失败"); 
	
		
		

}

/*******************************************************sys-clock   类型问题（看任务集）******************************************/

/****************************************************单个任务求最小值(energy_min_freq)********************************************/
double energy_min_freq(const taskset *task,int n)
{//S=slack,I=idle duration

  double Idle,beta,deta;
  Idle=beta=deta=0;
  double alpha=1.0;
  bool IN_BZP=true;
  int S,w,w_pie,t;
  w=task[n].c;
  t=S=w_pie=0;
  do 
  {
	  if(IN_BZP==true)
	  {
		  do{
		  for(int i=0;i<=n;i++)  //n的情况，数组从零开始
			 {
				w_pie+=task[i].t*(w/task[i].t+1);	//？
			}
			w_pie=w_pie+S;
			deta=w_pie-w;
			w=w_pie;
	  }while(w<task[n].d && deta>0);
	  IN_BZP=false;	
	  }
	  else 
	  {
		  //求Idle 找一个最小值
		  int min=task[0].t*(w/task[0].t+1);
		  for(int j=0;j<=n;j++)
		  {	
			int temp=task[j].t*(w/task[j].t+1);
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

        if(beta/t<alpha) alpha=beta/t;
        IN_BZP=true; 
	 
	  }			
      


  }while(w<task[n].d);
  


  return alpha;
}

/*****************把任务按deadline递减排列(未完成)***********************************/
int Partition_task(taskset *task,int low,int high)
{
	task[0]=task[low];
	double pivotkey=task[low].d;
	while(low<high){
		while(low<high && task[high].d>=pivotkey) --high;
		task[low]=task[high];
		while(low<high && task[low].d<=pivotkey)  ++low;
		task[low]=task[high];
	
	}
	task[low]=task[0];
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
void QuickSort_task(taskset *task)
{
	QSort_task(task,1,task_number);

}


/***************************************sys-clock 主体******************************************/
double sys_clock(const taskset *task)
{
	double temp[task_number];

	
	/*****************把任务按deadline递减排列(未完成)***********************************/
	void pailie();

	//求每个任务中的最小Fre
	for(int i=0;i<task_number;i++)
	{	
		
		temp[i]=energy_min_freq(task,i);
	
	}
	//找到所有任务中频率最高的
	for(int j=0;j<task_number;j++)
	{
		if(temp[j]>temp[j+1]) temp[j+1]=temp[j]; 
				
	}
	

	return temp[task_number-1];

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
	

	//返回处理器的序号-1
	return temp;
}

/***********************************快速排序（未完成）***************************************/
int Partition(taskset *task,int low,int high)
{
	task[0]=task[low];
	double pivotkey=task[low].uTask;
	while(low<high){
		while(low<high && task[high].uTask>=pivotkey) --high;
		task[low]=task[high];
		while(low<high && task[low].uTask<=pivotkey)  ++low;
		task[low]=task[high];
	
	}
	task[low]=task[0];
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
	QSort(task,1,task_number);

}





/**********************************************WFD主体*****************************/
void WFD(taskset *task,Processors *processor)
{
	/*************对任务集按利用率快速排序****************/
	
	//  void QuickSort(task);   定义完任务集，可以了

	



	/***********************主体**************************/ 
	//temp为最小的处理器的序号-1
	int temp;
	/*****************分派过程***************************/
	for(int i=0;i<task_number;i++)
	{   //task_u为任务的利用率	
		double task_u=task[i].uTask;
	   
		temp=min_processor(processor,ProcessorUsedNumber);     //第2个参数需要想想
		if((processor[temp].u+task_u)<1 && processor[temp].state==true) {
			
			processor[temp].task_in[i]=(i+1);
			processor[temp].u=processor[temp].u+task_u;
		}
		else
		{
			
			processor[ProcessorUsedNumber].state=true;
			processor[ProcessorUsedNumber].task_in[i]=(i+1);   //找一个为state为false的处理器的序号？   利用一个processorUsedNumber已解决
			processor[ProcessorUsedNumber].u=task_u;
			ProcessorUsedNumber+=1;
		}
	
	}


}

  


void main()
{	


}
