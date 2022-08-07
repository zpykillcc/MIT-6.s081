#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primeproc(int *fd){
    int p;
    read(fd[0],&p,sizeof(p));
    if (p == -1){
        exit(0);
    }
    fprintf(1,"prime %d\n",p);
    
    int pipefd[2];
    pipe(pipefd);
    int pid = fork();
    if (pid == 0){ // child process
        close(pipefd[1]);
        primeproc(pipefd);
    }
    else { // parent process
        close(pipefd[0]);
        int buf;
        while (read(fd[0], &buf, sizeof(buf)) && buf !=-1){ //读入左进程
            if (buf % p != 0){
                write(pipefd[1],&buf,sizeof(buf)); //将筛出数的传入右进程
            }
        }
        buf = -1;
        write(pipefd[1], &buf, sizeof(buf));
        close(pipefd[0]);close(pipefd[1]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[]){
    int default_pipe[2];
    pipe(default_pipe);
    int pid = fork();
    if (pid == 0){
        close(default_pipe[1]);
        primeproc(default_pipe);
        exit(0);
    }
    else {
        close(default_pipe[0]);
        for (int i=2; i<=35; i++)
            write(default_pipe[1], &i, sizeof(i));
        int i=-1;
        write(default_pipe[1], &i, sizeof(i));
        close(default_pipe[1]);
        wait(0);
        exit(0);
    }
}