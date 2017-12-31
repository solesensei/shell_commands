#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    if (argc < 3)
    { 
        perror("not enough arguments"); 
        return 1;
    }
    
    int flag = *argv[1] == '-' ? 1 : 0; //check on flags
    char* from = argv[flag+1];
    char* to = argv[flag+2];

    int fd_from,fd_to;
    char buf[4096];
    ssize_t rd;
    fd_from = open(from, O_RDONLY);
    fd_to = creat(to, 0666);
    if (fd_from < 0 || fd_to < 0){
        if (fd_to >= 0) close(fd_to);
        perror("open err");
        return 1;
    }
    while(rd = read(fd_from,buf,sizeof buf),rd>0)
    {
        char *buf_ptr = buf;
        ssize_t wr;
        /*write -> decrease rd & increase ptr offset on write size*/
        do{
            wr = write(fd_to,buf_ptr,rd);
            if(wr >= 0)
            {
                rd -= wr;
                buf_ptr += wr;
            }
            else { perror("write err"); return 1; }
        }while (rd > 0);
    }
    
    if(rd == 0) 
    {
        if (close(fd_to) < 0)
            perror("close err");
        close(fd_from);
    }
    else perror("read err");
    return 0;
}

