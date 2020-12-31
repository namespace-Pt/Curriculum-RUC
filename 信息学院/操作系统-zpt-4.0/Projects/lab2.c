#include<stdio.h>
#include<pthread.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
//解决wait的warning
#include <sys/wait.h>

int istoWait = 1;
int fd[2];
pid_t pid1,pid2;
char info_1[50] = "Child Process 1 is sending a message!\n";
char info_2[50] = "Child Process 2 is sending a message!\n";

void waiting(){
    while (istoWait == 1)
    {
        /* code */
    }
}

void deposit_ESC_1(int what){
    istoWait = 0;
    printf("Child Process 1 is Killed by Parent!\n");
    

    close(fd[0]);
        //向管道写入信息并关闭管道
        //用循环写入，这样方便对比加锁前后的输出情况
        lockf(1,1,0);
        for(int i = 0;i < strlen(info_1);i++){
            //printf("%c",info_1[i]);
            write(fd[1],info_1+i,1);
        }

        //write(fd[1],info_1,strlen(info_1));
        close(fd[1]);
        lockf(1,0,0);
        kill(pid2,17);
        exit(0);
} 
void deposit_ESC_2(int what){
    istoWait = 0;
    printf("Child Process 2 is Killed by Parent!\n");
    close(fd[0]);
            //写入管道并关闭通道
            //同上
            lockf(1,1,0);
            for(int i = 0;i < strlen(info_2);i++){
                //printf("%c",info_2[i]);
                write(fd[1],info_2+i,1);
            }
            //write(fd[1],info_2,strlen(info_1));
            
            close(fd[1]);
            lockf(1,0,0);
            exit(0);
} 
void deposit_ESC_P(int what){
    istoWait = 0;
     
    kill(pid1,16);

    //用sleep和信号都可以完成两个子进程的顺序
    //sleep(1);
    //kill(pid2,17);
} 

int main(){
    //int fd_2[2];
    if (pipe(fd) == -1)
    {
        printf("Error in creating pipes");
    }
    
    pid1 = fork();
    //int pid2 = fork();
    switch (pid1)
    {
        
    case 0/* constant-expression */:
        istoWait = 1;
        //忽视键盘中断，否则无法跳出循环
        signal(SIGINT,SIG_IGN);
        //关联自定义信号
        signal(16,deposit_ESC_1);
        printf("Child1\n");
        
        waiting();

        //关闭读入管道
        
        //lockf(1,0,0);
        break;
        
    case -1:
        printf("Error in creating process1\n");
        break;
    default:
        printf("Parent\n");
        //创建读入缓存区
        char buff[100];
        
        //if (pipe(fd_2) == -1)
        //{
        //    printf("Error in creating pipes");
        //}
        pid2 = fork();
        switch (pid2)
        {
        case 0:
            istoWait = 1;
            //关联信号
            signal(SIGINT,SIG_IGN);
            signal(17,deposit_ESC_2);
            printf("Child2\n");
            //等待键盘指令
            waiting();
            //关闭读入通道
            
            
            break;
        case -1:
            printf("Error in creating process2\n");
        default:
            
            //不做处理，因为和下面一样都是父进程
            break;
        }
        //关闭写入通道
        close(fd[1]);
        
        //关联信号，一定要把sigint忽视
        signal(SIGINT,deposit_ESC_P);
        //等待键盘指令
        waiting();
        //接受到ctrlC后发送信号
        
        //等待子进程结束
        //必须用循环，否则等待一个的话会造成僵尸进程和输出问题
        while(-1 != wait(0)){
            ;
        }
        //从同一个buff中读取信息
        read(fd[0],buff,sizeof(buff));
    
        //写入输出中
        write(STDOUT_FILENO,buff,strlen(info_1)+strlen(info_2));

        printf("Parent Process is killed!\n");
        exit(0);
        break;
    }
    return 0;
}