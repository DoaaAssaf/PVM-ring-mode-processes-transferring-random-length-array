#include <stdio.h>
#include <stdlib.h>


#include "pvm3.h"

   int para[4];
   int n;
int prev_tid,next_tid;




void ReadParameters(){

  int RingMode;
  int MimLenght;
  int n;
  int MaxLenght;
  
  


  printf("enter the ring mode :  1 for forword mode and amy number for backword \n ");
  scanf("%d",&RingMode);
	  para[1]=RingMode;
  


  printf("enter the min length of array \n");
  scanf("%d",&MimLenght);
  para[2]=MimLenght;

  printf("enter the max length of array \n");
  scanf("%d",&MaxLenght);
  para[3]=MaxLenght;

  printf("enter the sons number??? \n");
  scanf("%d",&n); 
  para[4]=n;
  




}

void NextPrev(int RingMode){
    int* siblingIds=0;
	int siblingCount,i,me;
	 siblingCount = pvm_siblings(&siblingIds);
	 for(i=0; i<siblingCount; i++) {
			if(siblingIds[i] == pvm_mytid())
			{ 
				me = i;			
				if ((i == 0) && (RingMode==1)){
				    next_tid = siblingIds[i+1];
					prev_tid = siblingIds[siblingCount -1];
				}
				else if ((i == 0) && (RingMode != 1)){
			     	next_tid = siblingIds[siblingCount -1]; 
					prev_tid =siblingIds[i+1];
					} 
			  else if ((i == siblingCount -1) && (RingMode == 1)){
			     	next_tid = siblingIds[0]; 
					prev_tid =siblingIds[siblingCount-2];
					}
              else if ((i == siblingCount -1) && (RingMode != 1)){
			     	next_tid = siblingIds[siblingCount-2];
					prev_tid = siblingIds[0];
					}
			  else if (RingMode == 1){
				    next_tid = siblingIds[i+1];
                    prev_tid = siblingIds[i-1];
			  }
               else if (RingMode != 1){
				    next_tid = siblingIds[i-1];
                    prev_tid = siblingIds[i+1];
			  }
		}
	 }
}




void  ParentWork(int tid){
	  
	int  i;
	
	int buf,tag;
	int rec_array_length=5;
	int* fdata;	
	int* children;
	fdata = (int*) malloc(rec_array_length * sizeof(int));	
	


    ReadParameters();
    printf("This is a Master Task its tid = %x \n",tid);;
 	children = (int*) malloc(para[4] * sizeof(int));
	
	//pvm_catchout(stdout);
	i = pvm_spawn("stride", 0, 0, "", para[4], children);
	if(i != para[4])
		{
			printf("Invalid Children count\n");
			
		}
	
	printf("Parent %d Spawned Sucsessfully the following Chlidren\n", tid);		

	for(i=0; i<para[4]; i++)
		
		printf("\tChild [%d] ID = %d\n",i, children[i]);
    
	    printf("____________________________\n");
     

        for (i=0;i<para[4];i++){
     	pvm_initsend(PvmDataDefault);
        pvm_pkint(para,4,1);
		pvm_pkint(&children[0],1,1);
         pvm_send(children[i], 10);

       		}



		for(i=0; i< 2*para[4]; i++){
           buf= pvm_recv(-1, -1);		 
		   pvm_bufinfo(buf, 0, &tag, &tid);
	   	   if(tag == 3000)
			{	
			   pvm_upkint(&rec_array_length,1,1);
			    	printf("child [ %d ] received array length = %d \n ",tid,rec_array_length);
					pvm_upkint(fdata,rec_array_length,1);
			    	printf("child [ %d ] recieved  array  =",tid);
					for (i=0;i<rec_array_length;i++)
					{
					  printf( " %d ",fdata[i]);
					}
					printf("\n _____________ \n");
		   }		
     
			
			else  if(tag == 4000)
			{	
				pvm_upkint(&rec_array_length,1,1);
		        	printf("child [ %d ] created array length = %d \n ",tid,rec_array_length);
					 pvm_upkint(fdata,rec_array_length,1);
			    	printf("child [ %d ] created  array  =",tid);
					for (i=0;i<rec_array_length;i++)
					{
					  printf( " %d ",fdata[i]);
					}
					printf("\n _____________ \n");
			}
		
				else  if(tag == 5000)
			{	
				pvm_upkint(&rec_array_length,1,1);
		        	printf("child [ %d ] created array length = %d \n ",tid,rec_array_length);
					 pvm_upkint(fdata,rec_array_length,1);
			    	printf("child [ %d ] created  array  =",tid);
					for (i=0;i<rec_array_length;i++)
					{
					  printf( " %d ",fdata[i]);
					}
					printf("\n _____________ \n");
			}
		} 

}

int Random1 (int Min,int Max){

    int x=Max-Min;
return (int) (((double)(x+1)/RAND_MAX)*rand()+Min);

}


void ChildWork(int pid,int tid){
   
   int ran=6;
   int rani;
  // int rec_array_length;
   int i;
  // int random_val;
   int* fdata;
   int* rec_data;
   int fch;
   int data_length=5;
 
  
   rec_data = (int*) malloc(data_length * sizeof(int));
   fdata = (int*) malloc(ran * sizeof(int));
  
    //recieve from parent parameters array

	pvm_recv(pid, 10);
	pvm_upkint(para,4, 1);
	pvm_upkint(&fch,1, 1);

    //define who is next 
    NextPrev(para[1]);





	if (tid == fch ){
    
       srand(pvm_mytid());
      rani= rand() % (para[3]-para[2]+1) +para[2];
        for (i=0;i<rani;i++){
           fdata[i]=Random1(i,i+150);
		}
        pvm_initsend(PvmDataDefault);
        pvm_pkint(&rani,1,1);
        pvm_pkint(fdata,rani,1);
        pvm_send(next_tid, 10);

	    pvm_initsend(PvmDataDefault);
        pvm_pkint(&rani,1,1);
        pvm_pkint(fdata,rani,1);
        pvm_send(pid, 4000);

        pvm_recv(prev_tid, 10);
        pvm_upkint(&data_length,1, 1);
        pvm_upkint(rec_data,data_length, 1);
	 
        pvm_initsend(PvmDataDefault);
	    pvm_pkint(&data_length,1,1);
        pvm_pkint(rec_data,data_length,1);
     	pvm_send(pid, 3000);
  
	
}

else {

    
   // ran= rand() % (para[3]-para[2]+1) +para[2];
   //  srand(pvm_mytid());
	pvm_recv(prev_tid, 10);
	pvm_upkint(&data_length,1, 1);
    pvm_upkint(rec_data,data_length, 1);


    srand(pvm_mytid());
   ran= rand() % (para[3]-para[2]+1) +para[2];

	if (data_length == ran){
		 for (i=0;i<=ran;i++){
			fdata[i]=rec_data[i];
			}
		}
	else if  (data_length > ran){ 
		     	for (i=0;i<=ran;i++){
		        	fdata[i]=rec_data[i];
				}
	
		}
    else {
	     	for (i=0;i<=data_length;i++){
		     	fdata[i]=rec_data[i];
			}
            for (i=data_length+1;i<=ran ;i++){
                fdata[i]=Random1(0,1000);
				
			}	
	
		}

        

    pvm_initsend(PvmDataDefault);
	pvm_pkint(&data_length,1,1);
    pvm_pkint(rec_data,data_length,1);
    pvm_send(pid, 3000);
   

    
       

    pvm_initsend(PvmDataDefault);
    pvm_pkint(&ran,1,1);
	pvm_pkint(fdata,ran,1);
    pvm_send(next_tid, 10);

	 pvm_initsend(PvmDataDefault);
    pvm_pkint(&ran,1,1);
	pvm_pkint(fdata,ran,1);
    pvm_send(pid, 5000);
}
}





int main(int argc, char** argv )
{

   	int   tid = pvm_mytid();
    int   pid = pvm_parent();
	


	if(pid == PvmNoParent || pid == PvmParentNotSet) //Parent
	{
		    ParentWork(tid); 
	}

	else //Child
	{ 
      ChildWork(pid,tid);
	
	}
		
	
	pvm_exit();
	return 0;
}
