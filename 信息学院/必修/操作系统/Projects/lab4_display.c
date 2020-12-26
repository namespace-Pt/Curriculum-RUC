#include"lab4_pre.c"
int main(){
    int msg_id = getMsgQueue();
    int shm_id = initShm(IPC_CREAT);
    struct shmItem * shared = getShm();
    printf("Process Display\n");
    while (1)
    {
        char out[100] = {};
        recvMsg(msg_id,(long)INPUT,out);
        if(!strcmp("quit",out)){
            printf("message queue deleted(need to be deleted manually in shell)");
            destroyMsgQueue(msg_id);
            return 0;
        }
        printf("output:%s\n",out);
        while(!shared->processed){}
        if(shared->processed == 1){
            printf("Inputting Data has been Processed\n");
        }
        else
        {
            printf("Shared Memory Destroyed");
            destroyShm(shm_id,shared);
            return 0;
        }
    }
}