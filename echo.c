#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int f_check(char* arg) // flags
{
    char sym = arg[1];
    switch (sym){
        case 'n': return 1;
        case 'e': return 2;
        case 'E': return 3;
    }

    return 0;
}

int to_oct_hex (char** s, char* arg, const char sym)
{
    int size, seq = 0;
    int i,k;
    char c;
    const char allow[] = "0123456789ABCDEFabcdef";
    int exist = 0;
    int mode = sym == '0' ? 0 : 1;
    int msize = 3 - mode; //max size of digits
    for(i = 1; i<=msize; ++i){
        for(k = 0; (mode && allow[k]) || k < 10; ++k) // mode: hex | oct
            if (arg[i] == allow[k]){
                exist = 1;
                c = k>10 ? 'a': '0'; //for hexad - letter or number
            }
        
        if (exist){ //symbol
            ++size;
            exist = 0;
            if (mode) 
                 seq = seq*16 + tolower(arg[i])-c;
            else seq = seq*8+arg[i]-'0';
        }
        else break;
        
    }
    **s = seq;
   
    return size;

}

int arg_write(char* arg, int flg_e) //return esc_c
{
    char* buf = malloc (sizeof(char)*3);
    int buf_size = 2;
    int off_size = 1;
    int esc_c = 0; // '\c'
    int i, esc_s;
    esc_s = 0;
    char sym = '1';
    int part = 0;
    int offs = 0;
    for(i = 0; sym != '\0'; ++i){
        sym = arg[i];
        if (sym == '\\'){ esc_s = 1; continue; }
        else ++part;
        
       
        if (esc_s && flg_e)
         switch (sym){
            case 'a': *buf='\a'; break;
            case 'b': *buf='\b'; break;
            case 'c': esc_c = 1; break; //break for()
            //case 'e': *buf='\e'; break; //unknown escape sequences
            case 'f': *buf='\f'; break;
            case 'n': *buf='\n'; break;
            case 'r': *buf='\r'; break;
            case 't': *buf='\t'; break;
            case 'v': *buf='\v'; break;
            case 'x': case '0': off_size = to_oct_hex(&buf,arg+i,sym); buf_size = 1; i = i + off_size; break; 
            default : esc_s = 0; 

         }
        
        if (esc_c) break;
        if (esc_s && flg_e){ 
            write(1,arg+offs,part-1);
            write(1,buf,buf_size); 
            offs = i+1;
            buf_size = 2; off_size = 1;
            part = 0; esc_s = 0;
        }

    }

   write(1,arg+offs,part-1);

   free(buf);
   return esc_c;
}


int main (int argc, char *argv[])
{
    int i = 1;
    int flag = 0; // no flags
    int maxc = argc-1;
    int masf[4] = {0,0,0,0}; //massive of flags;
    if (argc > 1) //check on flags
    for(i = 1; maxc > 0 && *argv[i] == '-'; ++i, --maxc){
         flag = f_check(argv[i]); 
         masf[flag] = 1;
         if (flag == 3) masf[2] = 0;
    }
    for( ; i<argc; ++i){
        if ( arg_write (argv[i],masf[2]) ) 
        {
            masf[1] = 1; //no '\n'
            break;
        }
        if (i != argc-1) write(1," ",1);
    }
    if(masf[1] == 0) write(1,"\n",1);

    

    return 0;

}
