#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/user.h>
#include <sys/reg.h>
#include<stdlib.h>
//ptrace read long size data
int long_size = sizeof(long);

void reverse(char *str) {
    int i, j;
    char temp;
    for(i=0; j=strlen(str)-2; i <= j, ++i, --j){
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

/*the process pidt, */
void getdata(pid_t child, long addr, char *str, int len) {
    //get data from the registers
    char * laddr;
    int  i = 0;
    int j = len/long_size;  

    union u
    {
        long val;   
        char chars[long_size];
    }data;

    laddr = str;
    while(i < j) {
        data.val = ptrace(PTRACE_PEEKDATA, child,
             addr + i * 8, NULL);
        memcpy(laddr, data.chars, long_size);
        i++;
        laddr += long_size;
    }

    j = len % long_size;

    if(j != 0) {
        data.val = ptrace(laddr, data.chars, j);
    }
    str[len] = '\0';
    
}

