#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define INPUT 1

struct msgItem
{
    long type;
    char text[1024];
}msgItem = {0,{0}};

struct shmItem
{
    int processed;
    char text[1024];
}shmItem = {0,{0}};

int initMsgQueue(int status){
    key_t key = ftok("./",0);
    if(key < 0){
        perror("failure in creating key");
    }
    int msg_id = msgget(key,status);
    if(msg_id < 0){
        perror("failure in creating message queue");
    }
    return msg_id;
}
int createMsgQueue(){
    return initMsgQueue(IPC_CREAT|0666);
}
int getMsgQueue(){
    return initMsgQueue(IPC_CREAT);
}
int destroyMsgQueue(int msg_id){
    if(msgctl(msg_id,IPC_RMID,NULL) < 0){
        perror("failure in destrpying message queue");
    }
    return 0;
}
int sendMsg(int msg_id,int type,char* message){
    struct msgItem  msgitem;
    msgitem.type = type;
    strcpy(msgitem.text,message);
    if(msgsnd(msg_id,(void*)&msgitem,sizeof(msgitem.text),0) < 0){
        perror("error in sending message");
        return -1;
    }
    return 0;
}

int recvMsg(int msg_id,int type,char * out){
    struct msgItem recvitem;
    int size = sizeof(recvitem.text);
    if(msgrcv(msg_id,(void*)&recvitem,size,type,0) < 0){
        perror("error in receiving message");
        return -1;
    }

    strcpy(out,recvitem.text);
    return 0;
}

int initShm(int status){
    key_t key = ftok("./",0);
    if(key < 0){
        perror("failure in creating key");
    }
    int msg_id = shmget(key,1024,status);
    if(msg_id < 0){
        perror("failure in creating shared memory");
    }
    //printf("%d",msg_id);
    return msg_id;
}

struct shmItem * createShm(){
    int shm_id = initShm(IPC_CREAT|0666);
    return (struct shmItem * )shmat(shm_id,0,0);
}

struct shmItem * getShm(){
    int shm_id = initShm(IPC_CREAT);
    return (struct shmItem * )shmat(shm_id,0,0);
}

int destroyShm(int shm_id,struct shmItem * shareMem){
    if(shmdt(shareMem) == -1){
        perror("Error in Unbounding Shared Memory");
    }
    if(shmctl(shm_id, IPC_RMID, 0) == -1){
        perror("Error in Destroying Shared Memory");
        return -1;
    }
};

int writeToShm(struct shmItem * shareMem,char * text){
    shareMem->processed = 0;
    strcat(shareMem->text,text);
    return 0;
}

int readFromShm(struct shmItem * shareMem,char out[]){
    shareMem->processed = 1;
    strcpy(out,shareMem->text);
    //puts(out);
    memset(shareMem->text,0,sizeof(out));
    return 0;
}