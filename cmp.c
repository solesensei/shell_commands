#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc,char* argv[])
{
    if (argc < 3)
        { perror("not enough arguments"); return 1; }
    int fd1,fd2;
    char* file1 = argv[1];
    char* file2 = argv[2];
    ssize_t rd1,rd2;
    fd1 = open(file1, O_RDONLY);
    if (fd1 < 0)
    { printf("%s: no such file in directory",argv[1]); return 1; }
    fd2 = open(file2, O_RDONLY);
    if (fd2 < 0)
    { printf("%s: no such file in directory",argv[2]); return 1; }
    char buf[2];
    char* p1 = buf;
    char* p2 = buf+1;
    int byte = 1;
    int line = 1;
    while( (rd1 = read(fd1,p1,1)) && (rd2 = read(fd2,p2,1)) )
    {
        if (*p1 == *p2){
            if (*p1 == '\n') ++line;
            ++byte;
        }
        else{
            printf("%s %s differ: byte %d, line %d\n",argv[1],argv[2],byte,line);
            close(fd1);
            close(fd2);
            return 0;
        }


    }

    return 0;
}
