#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<string.h>
#include<semaphore.h>


char inputBuff[100] = "";
pthread_mutex_t mutex_buff;
pthread_cond_t toDisplay;//toProcess;
sem_t toProcess,toNotify;//用条件变量的时候必须满足其和output进程的互斥，但是process进程和output进程没有互斥关系
pthread_t tid1,tid2,tid3;

void * KCP_input(){
    pthread_mutex_lock(&mutex_buff);
    printf("请输入数据：");
    scanf("%s",inputBuff);
    //pthread_cond_signal(&toDisplay);

    pthread_mutex_unlock(&mutex_buff);
    //条件变量的signal的位置要求不严格，因为其不涉及开锁再关锁的行为
    pthread_cond_signal(&toDisplay);
    sem_post(&toProcess);
    //pthread_cond_signal(&toProcess);
    return (void*)0;
}
void * DCP_output(){    
    //cond_wait必须放在互斥锁内部，否则由于其机制是收到相应条件变量后会再次把该锁锁上，会造成死锁
    //pthread_cond_wait(&toDisplay,&mutex_buff);
    pthread_mutex_lock(&mutex_buff);
    pthread_cond_wait(&toDisplay,&mutex_buff);
    printf("%s\n",inputBuff);
    pthread_mutex_unlock(&mutex_buff);
    printf("-------processing data--------\n");
    sem_wait(&toNotify);
    printf("Process completed!\n");
    return (void*)0;
}
void * CCP_dispose(){
    sem_wait(&toProcess);
    FILE * fp_num = fopen("numbers.txt","w");
    FILE * fp_let = fopen("letters.txt","w");
    //pthread_mutex_lock(&mutex_buff);
    //pthread_cond_wait(&toProcess,&mutex_buff);
    for(int i = 0;i < strlen(inputBuff);i++){
        int c = (int)inputBuff[i];
        if(c<= 57 && c>=48){
            fputc(inputBuff[i],fp_num);
        }
        else if(c<=122 && c>=65){
            fputc(inputBuff[i],fp_let);
        }
    }
    sem_post(&toNotify);
    //关闭文件
    fclose(fp_let);
    fclose(fp_num);
    return (void*)0;
}

int main(){

    pthread_mutex_init(&mutex_buff,NULL);
    //pthread_cond_init(&toProcess,NULL);
    pthread_cond_init(&toDisplay,NULL);
    sem_init(&toProcess,1,0);//同步
    sem_init(&toNotify,1,0);//同步

    
    //默认先创建的的线程会先执行
    pthread_create(&tid1,NULL,DCP_output,NULL);
    pthread_create(&tid2,NULL,CCP_dispose,NULL);
    pthread_create(&tid3,NULL,KCP_input,NULL);
    
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    //等待所有线程完成任务后释放内存
    pthread_cond_destroy(&toDisplay);
    pthread_mutex_destroy(&mutex_buff);
    sem_destroy(&toProcess);
    sem_destroy(&toNotify);
    
    return 0;
}