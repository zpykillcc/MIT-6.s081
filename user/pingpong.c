#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
        //文件描述符0为读取，1为写入
        char buf;
        int pipefd[2];
        if (pipe(pipefd) == -1){
            fprintf(2,"Cannot create pipe\n");
            exit(-1);
        }
        int pid = fork();
        if (pid < 0){
            fprintf(2,"Cannot fork a process\n");
            exit(-1);
        }

        if (pid > 0){ //parent process
            char sendBuf = 'p';
            write(pipefd[1],&sendBuf,sizeof(sendBuf));
            close(pipefd[1]);
            
            read(pipefd[0], &buf,sizeof(buf));
            fprintf(1,"%d: received pong\n",getpid());
            close(pipefd[0]);
            wait(0); //wait child process exit;
        } 
        else {  //child process
            char sendBuf = 'c';
            read(pipefd[0], &buf, sizeof(buf));
            fprintf(1,"%d: received ping\n", getpid());
            write(pipefd[1], &sendBuf, sizeof(sendBuf));
            
            close(pipefd[0]);
            close(pipefd[1]);
            exit(0);
        }
        exit(0);
}