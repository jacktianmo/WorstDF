#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
/*********************************************8***********������Ŵ�1��ʼ**********************
********************************************************����������Ŵ�1��ʼ******************
*************
*/




const int task_number=4;													//������Ŀ
double processor_number=2;													//��������Ŀ
int ProcessorUsedNumber=0;													//���õĴ���������Ŀ

struct Processors{

	double u;																//������
	int task_in[task_number];												//��������������š�0��ʾ������
	bool state;																//�������Ƿ�ʹ�� true�����ã�false��û����

};


/****************************************��������ʼ��*****************************************/
void process_init_(Processors *processor)
{
	for(int i=0;i<processor_number;i++)													//5Ϊ����������Ŀ;�ѽ��
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

taskset task[task_number];															//������һ������
/****************************************��������*******************************************/

void MakeTaskset()				//�����ܵ�������
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
		printf("��������%d��%d\n",i,task[i].c);
}




/*******************************************************sys-clock   �������⣨�����񼯣�******************************************/

/****************************************************������������Сֵ(energy_min_freq)********************************************/
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
		  for(int i=0;i<=n;i++)  //n�������������㿪ʼ
			 {
				w_pie+=task[i].t*(w/task[i].t+1);	//��
			}
			w_pie=w_pie+S;
			deta=w_pie-w;
			w=w_pie;
	  }while(w<task[n].d && deta>0);
	  IN_BZP=false;	
	  }
	  else 
	  {
		  //��Idle ��һ����Сֵ
		  int min=task[0].t*(w/task[0].t+1);
		  for(int j=0;j<=n;j++)
		  {	
			int temp=task[j].t*(w/task[j].t+1);
			if(temp<min)
			{
				min=temp;
				
					
			}//�����Ż�
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

/*****************������deadline�ݼ�����(δ���)***********************************/
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
void QuickSort_task_deadline(taskset *task)
{
	QSort_task(task,0,task_number-1);

}


/***************************************sys-clock ����******************************************/
double sys_clock(taskset *task)
{
	double temp[task_number];

	
	/*****************������deadline�ݼ�����(δ���)***********************************/
	QuickSort_task_deadline(task);

	//��ÿ�������е���СFre
	for(int i=0;i<task_number;i++)
	{	
		
		temp[i]=energy_min_freq(task,i);
	
	}
	//�ҵ�����������Ƶ����ߵ�
	for(int j=0;j<task_number;j++)
	{
		if(temp[j]>temp[j+1]) temp[j+1]=temp[j]; 
				
	}
	

	return temp[task_number-1];

}

/*************************************wfd�㷨*****************************************************/



//����С�������ʵĴ�����
int min_processor(const Processors *processor,int ProcessorUsedNumber)
{
	int temp=0;
	//�Ҵ������������ʵ���Сֵ

	for(int i=0;i<ProcessorUsedNumber;i++)
	{
		if(processor[i].u<processor[temp].u)
		{
			temp=i;
		}
	}
	
	printf("���ش���������ţ�%d\n",temp);
	//���ش����������-1
	return temp;
}

/***********************************��������δ��ɣ�***************************************/
int Partition(taskset *task,int low,int high)
{
	taskset temp=task[low];
	double pivotkey=task[low].uTask;
	while(low<high){
		while(low<high && task[high].uTask>=pivotkey) --high;
		task[low]=task[high];
		while(low<high && task[low].uTask<=pivotkey)  ++low;
		task[low]=task[high];
	
	}
	task[low]=temp;
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





/**********************************************WFD����*****************************/
void WFD(taskset *task,Processors *processor)
{
	/*************�����񼯰������ʿ�������****************/
	
	QuickSort(task);

	printf("�������\n");
	//test ����������
	for(int j=0;j<4;j++)
		printf("����%d\n",task[j].c);

	int ProcessorUsedNumber=0;										//���õĴ���������Ŀ����ʼ��Ϊ0


	/***********************����**************************/ 
	//tempΪ��С�Ĵ����������-1
	int temp;

	/*****************���ɹ���***************************/
	for(int i=0;i<task_number;i++)
	{   //task_uΪ�����������	
		double task_u=task[i].uTask;
	   
		temp=min_processor(processor,ProcessorUsedNumber);			 //��2��������Ҫ����
		printf("�õ��Ĵ�����Ϊ%d\n",temp);
		if((processor[temp].u+task_u)<1 && processor[temp].state==true) {
			
			processor[temp].task_in[i]=(i+1);
			processor[temp].u=processor[temp].u+task_u;
			
		}
		else
		{
			
			processor[ProcessorUsedNumber].state=true;
			printf("ǰ������ʹ��״����%d\n",ProcessorUsedNumber);
			processor[ProcessorUsedNumber].task_in[i]=(i+1);   //��һ��ΪstateΪfalse�Ĵ���������ţ�   ����һ��processorUsedNumber�ѽ��
			processor[ProcessorUsedNumber].u=task_u;
			ProcessorUsedNumber+=1;
			printf("������ʹ��״����%d\n",ProcessorUsedNumber);

		}
	
	}


}

  


void main()
{	
	MakeTaskset();
	Processors processor[2];
	process_init_(processor);
	//printf("��������״̬��%d\n",processor[0].state);
	WFD(task,processor);
	for(int i=0;i<2;i++)
	{	
		for(int j=0;j<4;j++)
		printf("��%d���������ֵ�������%d\n",i,processor[i].task_in[j]);	
	}

}
