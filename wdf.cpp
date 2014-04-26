#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
/*********************************************8***********������Ŵ�1��ʼ**********************
********************************************************����������Ŵ�1��ʼ******************
*************
*/




const int task_number=25;													//������Ŀ
double processor_number=5;													//��������Ŀ
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
void MakeTaskset(double total_ui)				//�����ܵ�������
{
	
		if(total_ui>1) printf("���������");
		int i=0;
		double max_ui=0.5;															//�����е����������������
		double temp_ui=0;															//�����ʵ���ʱ��������¼�ܵ�������
		while(i<task_number)
		{
			/*
			s=0.000;
			smax=0.00000;
			si=1.0000;
		    for
			*/	
			

			/****��ʼ������t*******/
			
			int temp_t;
			temp_t=rand()%3;
			switch (temp_t){
			case 0:task[i].t=1+rand()%10;break;        //���ڴ�1��10
			case 1:task[i].t=11+rand()%90;break;       //���ڴ�11��100
			case 2:task[i].t=101+rand()%900;break;     //���ڴ�101��1000
			}


			/**********��ʼ������ʱ��c**********/
			while(task[i].c<=(task[i].t*0.5))     
			{
				
			}
			
			/********��ʼ��deadline d************/
			/*������������deadline���*/
			task[i].d=task[i].d;

			/*�����������uTask*/
			//	task[i].uTask=task[i].c/task[i].t; �޸�
			temp_ui+=task[i].uTask;
			




				
				
		}
		/*�ж�temp_ui��total_ui���*/
		if(temp_ui==total_ui)
			printf("��������ɹ�");
		else
			printf("��������ʧ��"); 
	
		
		

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
void QuickSort_task(taskset *task)
{
	QSort_task(task,1,task_number);

}


/***************************************sys-clock ����******************************************/
double sys_clock(const taskset *task)
{
	double temp[task_number];

	
	/*****************������deadline�ݼ�����(δ���)***********************************/
	void pailie();

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
	

	//���ش����������-1
	return temp;
}

/***********************************��������δ��ɣ�***************************************/
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





/**********************************************WFD����*****************************/
void WFD(taskset *task,Processors *processor)
{
	/*************�����񼯰������ʿ�������****************/
	
	//  void QuickSort(task);   ���������񼯣�������

	



	/***********************����**************************/ 
	//tempΪ��С�Ĵ����������-1
	int temp;
	/*****************���ɹ���***************************/
	for(int i=0;i<task_number;i++)
	{   //task_uΪ�����������	
		double task_u=task[i].uTask;
	   
		temp=min_processor(processor,ProcessorUsedNumber);     //��2��������Ҫ����
		if((processor[temp].u+task_u)<1 && processor[temp].state==true) {
			
			processor[temp].task_in[i]=(i+1);
			processor[temp].u=processor[temp].u+task_u;
		}
		else
		{
			
			processor[ProcessorUsedNumber].state=true;
			processor[ProcessorUsedNumber].task_in[i]=(i+1);   //��һ��ΪstateΪfalse�Ĵ���������ţ�   ����һ��processorUsedNumber�ѽ��
			processor[ProcessorUsedNumber].u=task_u;
			ProcessorUsedNumber+=1;
		}
	
	}


}

  


void main()
{	


}
