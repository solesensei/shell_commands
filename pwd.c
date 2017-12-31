#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>

int main (int argc, char *argv[])
{
    char* buf  = malloc (sizeof(char)*PATH_MAX);
    if( (buf = getcwd(buf,PATH_MAX)) == 0 )
    { perror("getcwd error"); return 1; }
    write(1,buf,PATH_MAX);
    write(1,"\n",2);
    free(buf);
    
    return 0;

}
