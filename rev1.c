#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t mutex, wrt;
int sharedvar, rcount = 0;              //Shared Variable

void design()                 		//Design Function
{
    printf("--------------------------------------------------\n");
}

void *writer(void *arg)
{
        pthread_mutex_lock(&wrt);
        
	int n = rand() % 15;       	//Random in 15ns
        int d = ((int)arg);		
        
	    design();
        printf("W%d Wait for Random time between 0ns and 15ns = %d\n", d, n);
        sleep(n);                      //Delay generated time     
        printf("Enter the number of time W%d want to write:\n", d);
        int t;
        scanf("%d", &t);
        printf("Now W%d is writing i.e. ADDING\n", d);
        int j;
        for(j=0; j<t; j++){
                printf("Enter the %dth INTEGER value to write:\n", (j+1));
                int u;
                scanf("%d", &u);
                sharedvar = sharedvar + u;
        }
        printf("UPDATED value of Shared variable = %d \n", sharedvar);
        design();
        pthread_mutex_unlock(&wrt);
}

void *reader(void *arg){
    
        //Entry Part
    
        pthread_mutex_lock(&mutex);
        rcount++;
        
        if(rcount==1){
                pthread_mutex_lock(&wrt);   //No writer should come
        }
        pthread_mutex_unlock(&mutex);       //so next reader can come
    
        //Exit Part
    
        int n = rand() % 15;        //Random in 15ns
        int d = ((int)arg);
        printf("R%d wait for Random time between 0ns and 15ns = %d\n", d, n);
        sleep(n);
        printf("Enter the number of time R%d want to read:\n", d);
        int t;
        scanf("%d", &t);
        printf("Now R%d is reading\n", d);
        int j;
        for(j=0; j<t; j++){
                printf("R%d read the shared value = %d\n", d, sharedvar);
        }
        printf("Number of Readers present = %d\n", rcount);
        pthread_mutex_lock(&mutex);
        rcount--;
        if(rcount==0){//Now writer can come if they want
                pthread_mutex_unlock(&wrt);
        }
        pthread_mutex_unlock(&mutex);
}

void main(){
        printf("Enter the initial value of share variable: \n");
        scanf("%d", &sharedvar);
        design();
        int readerno, writterno, i;
        printf("Enter the no. of Reader:\n");
        scanf("%d", &readerno);
        for(i=0; i<readerno; i++){
                printf("R%d\n", i);
        }
        design();
        printf("Enter the no. of Writer:\n");
        scanf("%d", &writterno);
        for(i=0; i<writterno; i++){
                printf("W%d\n", i);
        }
        design();    
    
        pthread_t r[readerno], w[writterno];
        pthread_mutex_init(&wrt, NULL);
        pthread_mutex_init(&mutex, NULL);

        //Cases
    
        if(readerno<0 || writterno<0){
                printf("You have Entered NEGATIVE number of READER | WRITER\n");
                printf("Clossing\n");
                return;
        }else if(readerno == 0){
                printf("You have not taken any READER\n");
        }else if(writterno == 0){
                printf("You have not taken any WRITER\n");
        }else{
                printf("Creating Thread\n");
        }
        design();

        if(writterno==readerno){
                for(i=0; i<writterno; i++){
                        pthread_create(&w[i], NULL, &writer, (int *)i);
                        pthread_create(&r[i], NULL, &reader, (int *)i);
                }
                for(i=0; i<writterno; i++){
                        pthread_join(w[i], NULL);
                        pthread_join(r[i], NULL);
                }
        }else if(writterno>readerno){
                for(i=0; i<readerno; i++){
                        pthread_create(&w[i], NULL, &writer, (int *)i);
                        pthread_create(&r[i], NULL, &reader, (int *)i);
                }
                for(i=readerno; i<writterno; i++){
                        pthread_create(&w[i], NULL, &writer, (int *)i);
                }
                for(i=0; i<readerno; i++){
                        pthread_join(w[i], NULL);
                        pthread_join(r[i], NULL);
                }
                for(i=readerno; i<writterno; i++){
                        pthread_join(w[i], NULL);
                }
        }else{
                for(i=0; i<writterno; i++){
                        pthread_create(&w[i], NULL, &writer, (int *)i);
                        pthread_create(&r[i], NULL, &reader, (int *)i);
                }
                for(i=writterno; i<readerno; i++){
                        pthread_create(&r[i], NULL, &reader, (int *)i);
                }
                for(i=0; i<writterno; i++){
                        pthread_join(w[i], NULL);
                        pthread_join(r[i], NULL);
                }
                for(i=writterno; i<readerno; i++){
                        pthread_join(r[i], NULL);
                }
        }
        printf("After joining the thread\n");
        printf("Final value of share variable = %d\n", s);
}
