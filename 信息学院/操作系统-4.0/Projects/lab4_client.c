#include"lab4_pre.c"
int main(){
    int msg_id = createMsgQueue();
    struct shmItem * shared = createShm();
    //printf("%p",shared);
    printf("Process Client\n");
    char buf[100] = {0};
    while (1)
    {
        printf("type in message:\n");
        gets(buf);
        sendMsg(msg_id,(long)INPUT,buf);
        writeToShm(shared,buf);
        if(!strcmp("quit",buf)){
            return 0;
        }
    }
}