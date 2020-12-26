#include "lab4_pre.c"
int main(){
    struct shmItem * shared = getShm();
    
    FILE * fp_num = fopen("numbers.txt","w+");
    FILE * fp_let = fopen("letters.txt","w+");
    printf("Process Deposit\n");
    while (1)
    {
        if(shared->processed){
            continue;
        }
        char out[100] = {};
        readFromShm(shared,out);
        if(!strcmp(out,"quit")){
            shared->processed = 2;
            return 0;
        }
        
        //printf("%s",out);
        //fflush(stdout);
        

        for(int i = 0;i < strlen(out);i++){
            int c = (int)out[i];
            //printf("%d",c);
            if((c<= 57 && c>=48)||c == 10){
                fputc(out[i],fp_num);
                fflush(fp_num);
            }
            else if((c<=122 && c>=65)||c==10){
                fputc(out[i],fp_let);
                fflush(fp_let);
            }
        }
        fputc('\n',fp_num);
        fputc('\n',fp_let);
        fflush(fp_num);
        fflush(fp_let);
    }
}