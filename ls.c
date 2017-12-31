#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>

#ifndef  MAJOR
  #define MINOR_BITS 1
  #define MAJOR(dev) ((unsigned) dev >> MINOR_BITS)
  #define MINOR(dev) (dev & MINOR_BITS)
#endif

typedef struct node{
    char* name;
    struct node* next;
}node;

 
typedef struct glob_args{
    int flg_l;
    int flg_r;
    int flg_g;
    int flg_a;
    char** input_dir;
} glob_args;

glob_args ls_args = {0, 0, 0 ,0, NULL};


void f_check(int argc, char* argv[]) // flags
{
    int opt = 0;
    while( (opt = getopt(argc,argv,"laRg")) != -1 )
    {
        switch(opt){
        case 'l': ls_args.flg_l = 1; break;
        case 'r': ls_args.flg_r = 1; break;
        case 'g': ls_args.flg_g = 1; break;
        case 'a': ls_args.flg_a = 1; break;
        }
    }
    ls_args.input_dir = argv + optind;
}
void print_ftype (int st_mode) 
{                                   
    switch ( st_mode & S_IFMT ){ //mask file types
        case S_IFDIR:  putchar ( 'd' ); return; //catalog  
        case S_IFCHR:  putchar ( 'c' ); return; //byte
        case S_IFBLK:  putchar ( 'b' ); return; //block
        case S_IFREG:  putchar ( '-' ); return; //std
        case S_IFLNK:  putchar ( 'l' ); return; //link
        case S_IFSOCK: putchar ( 's' ); return; //socket
    }
} 
void print_permission (int st_mode)
{
    static const char xtbl[10] = "rwxrwxrwx";
    char amode[10];
    int i,j;

    for ( i=0, j=(1 << 8); i < 9; i++, j >>= 1 )
        amode[i] = ( st_mode&j ) ? xtbl[i]: '-';
    if ( st_mode & S_ISUID ) amode[2]= 's'; //user id
    if ( st_mode & S_ISGID ) amode[5]= 's'; //group id
    if ( st_mode & S_ISVTX ) amode[8]= 't'; //modifications only by owner
    amode[9]='\0';
    printf ( "%s ",amode );
}
void list_file(char* dr)
{
    DIR *d;
   // node list_dir;
    struct stat statv;
    struct passwd  *pw_d;
    struct dirent *dd;
    if ( !(d = opendir(dr)) ){ perror(dr); return; }
    while( (dd = readdir(d)) ) {
        if (*(dd->d_name)=='.' && !ls_args.flg_a) continue;
        printf("%s\n",dd->d_name);
        if (ls_args.flg_l)
        {
            if ( lstat (dd->d_name, &statv) )
                { perror (dd->d_name); return; }
            print_ftype ( statv.st_mode );
            print_permission ( statv.st_mode );
            printf ("%lu ",statv.st_nlink);  // number of hard links
            pw_d = getpwuid (statv.st_uid); // UID -> username
            printf ("%s ",pw_d->pw_name);
            //  if ( !(d = opendir(dr)) ){ perror(dr); return 1; }
            /*major and minor bits*/
            if ( (statv.st_mode & S_IFMT) == S_IFCHR || 
                 (statv.st_mode & S_IFMT) == S_IFBLK)
            printf ("%u, %lu",MAJOR(statv.st_rdev), MINOR(statv.st_rdev) );
            else
            printf ("%7ld", statv.st_size);
            printf ("     %s\n", dd->d_name);
        }
        else
            printf ("%s ", dd->d_name);
        
    } 
   if(!ls_args.flg_l) printf ("\n");
}

int main (int argc, char *argv[])
{
    f_check(argc,argv);    
    char* dr = "./"; 
    if (optind < argc)
     dr = argv[optind]; //just one dir
    printf("%s ",argv[optind]);
    list_file(dr);

    return 0;

}
