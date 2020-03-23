#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t mutex, wrt;
int s, rcount = 0;

void *writer(void *arg)
{
        pthread_mutex_lock(&wrt);
        
	    int waittime = rand() % 10;
        int d = ((int)arg);
        
	printf("--------------------------------------------------\n");
        printf("W%d Wait for Random time= %d\n", d, waittime);
        sleep(waittime);
        printf("Enter the number of time W%d want to write:\n", d);
        int t;
        scanf("%d", &t);
        printf("Now W%d is writing... i.e. ADDING...\n", d);
        int j;
        for(j=0; j<t; j++){
                printf("Enter the %dth value to write:\n", (j+1));
                int u;
                scanf("%d", &u);
                s = s + u;
        }
        printf("UPDATED value of Shared variable = %d \n", s);
        printf("--------------------------------------------------\n");
        pthread_mutex_unlock(&wrt);
}

void *reader(void *arg){
        //Entry Part
        pthread_mutex_lock(&mutex);
        rcount++;
        if(rcount==1){
                pthread_mutex_lock(&wrt);//No writer should come
        }
        pthread_mutex_unlock(&mutex);//so next reader can come
        //Exit Part
        int waittime = rand() % 10;
        int d = ((int)arg);
        printf("R%d wait for Random time= %d\n", d, waittime);
        sleep(waittime);
        printf("Enter the number of time R%d want to read:\n", d);
        int t;
        scanf("%d", &t);
        printf("Now R%d is reading....\n", d);
        int j;
        for(j=0; j<t; j++){
                printf("R%d read the shared value = %d\n", d, s);
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
        printf("Enter the Initial value: \n");
        scanf("%d", &s);
        printf("---------------------------------------------\n");
        int readerno, writerno, i;
        printf("no. of Reader:\n");
        scanf("%d", &readerno);
        for(i=0; i<readerno; i++){
                printf("R%d\n", i);
        }
        printf("---------------------------------------------\n");
        printf("no. of Writer:\n");
        scanf("%d", &writerno);
        for(i=0; i<writerno; i++){
                printf("W%d\n", i);
        }
        printf("---------------------------------------------\n");

        pthread_t r[readerno], w[writerno];
        pthread_mutex_init(&wrt, NULL);
        pthread_mutex_init(&mutex, NULL);

   
        printf("---------------------------------------------\n");

        if(writerno==readerno){
                for(i=0; i<writerno; i++){
                        pthread_create(&w[i], NULL, &writer, (int *)i);
                        pthread_create(&r[i], NULL, &reader, (int *)i);
                }
                for(i=0; i<writerno; i++){
                        pthread_join(w[i], NULL);
                        pthread_join(r[i], NULL);
                }
        }else if(writerno>readerno){
                for(i=0; i<readerno; i++){
                        pthread_create(&w[i], NULL, &writer, (int *)i);
                        pthread_create(&r[i], NULL, &reader, (int *)i);
                }
                for(i=readerno; i<writerno; i++){
                        pthread_create(&w[i], NULL, &writer, (int *)i);
                }
                for(i=0; i<readerno; i++){
                        pthread_join(w[i], NULL);
                        pthread_join(r[i], NULL);
                }
                for(i=readerno; i<writerno; i++){
                        pthread_join(w[i], NULL);
                }
        }else{
                for(i=0; i<writerno; i++){
                        pthread_create(&w[i], NULL, &writer, (int *)i);
                        pthread_create(&r[i], NULL, &reader, (int *)i);
                }
                for(i=writerno; i<readerno; i++){
                        pthread_create(&r[i], NULL, &reader, (int *)i);
                }
                for(i=0; i<writerno; i++){
                        pthread_join(w[i], NULL);
                        pthread_join(r[i], NULL);
                }
                for(i=writerno; i<readerno; i++){
                        pthread_join(r[i], NULL);
                }
        }
        printf("-------------After joining the thread---------\n");
        printf("Final value of share variable = %d\n", s);
}
