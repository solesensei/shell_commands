#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

void num_to_str(int,char(*)[]);
void cout(int fd, int flag)
{
    ssize_t wr,rd;
    int num = 1;
    char _num[] = "     1: ";
    char buf[100];
    char *p = buf;
    switch(flag){
    case 0:
    while(rd = read(fd,buf,sizeof(buf)),rd>0)
    {
        do{
            wr = write(1,p,rd);
            if(wr>=0)
            {
                rd -= wr;
                *p += wr;
            }   
            else { perror("write err"); exit(1); }
        }while(rd > 0);
    } 
    break;
    case 1:
    write(1,_num,8);
    while(rd = read(fd,buf,1),rd>0)
    {
        wr = write(1,p,1);
        if(wr < 0) { perror("write err"); exit(1); }
        if(*p == '\n'&& read(fd,buf,1)) 
        {
            num++;
            num_to_str(num,&_num);
            write(1,_num,8);
            lseek(fd,-1,SEEK_CUR);
        }
    }
    if(rd != 0) { perror("read err"); exit(1); } //for both cases
    }

}

void num_to_str(int num, char(* str)[])
{
    int i;
    int dig;
    char sym;
    for(i = 0; num > 0; ++i)
    {   
        dig = num % 10;
        sym = (char) dig + '0'; //digit to char '0'-'9'
       (*str)[5-i] = sym;
        num /= 10;
    }
}



int main(int argc, char* argv[])
{
    if(argc < 2)
    { perror("not enough arguments"); return 1; }
    int fd, i;
    int flag = *argv[1] == '-' ? 1 : 0; // -n flag (only one)
    for(i = 1+flag; i < argc; ++i){
        char* file = argv[i];
        fd = open(file,O_RDONLY);
        if (fd < 0)
        { 
            printf("%s: no such file in directory",argv[i]);
            return 1;
        }
        cout(fd,flag);
        close(fd);
    }

    return 0;
}
