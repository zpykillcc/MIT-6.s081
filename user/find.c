#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *target_file){
    int fd;
    struct stat st;
    struct dirent de;

    if ((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot access path %s\n",path);
        return;
    }

    if ((fstat(fd, &st) < 0) || st.type != T_DIR){
        fprintf(2, "find: cannot stat path %s\n", path);
        return;
    }
    char buf[512], *p; // 参考 ls.c
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      return;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        switch (st.type)
        {
        case T_FILE:
            if (!strcmp(de.name, target_file)){
                fprintf(1,"%s\n", buf);
            }
            break;
    
        case T_DIR:
            find(buf, target_file);
            break;
        }
    }
    return;
}

int main(int argc, char *argv[]){

    if (argc < 3){
        fprintf(2, "Usage: find dir filename\n");
        exit(0);
    }

    char *base_dir = argv[1];
    char *target_file = argv[2];
    find(base_dir, target_file);

    exit(0);
}