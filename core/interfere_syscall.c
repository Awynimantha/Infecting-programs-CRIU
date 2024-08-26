#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/user.h>
#include <sys/reg.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
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
        data.val = ptrace(PTRACE_PEEKDATA,
                          child, addr + i * 8,
                          NULL);
        memcpy(laddr, data.chars, j);
    }
    str[len] = '\0';
    
}

void putdata(pid_t child, long addr, char *str, int len) {
    char * laddr = str;
    int i = 0;
    int j = len/long_size;
    union u {
        long val;
        char chars[long_size];
    } data;

    while( i < j) {
        memcpy(data.chars, laddr, long_size);
        ptrace(PTRACE_POKEDATA, child, addr + i * 8, NULL);
        i++;
        laddr += long_size;
    };

    j = len % long_size;

    if(j != 0) {
        memcpy(data.chars, laddr ,j);
        ptrace(PTRACE_POKEDATA,
                          child, addr + i * 8,
                          NULL);
    }
    str[len] = '\0';


}

int main() {
        pid_t child;

        int status;
        struct user_regs_struct regs;
        long orig_rax;
        int insyscall = 0;
        long params[3];
        char * str;
        child = fork();

        if(child == 0) {
            ptrace(PTRACE_TRACEME, 0, NULL, NULL);
            execl("/bin/ls","ls", NULL);

        } else {
            while(1) {
                waitpid(child, &status, 0);
                orig_rax = ptrace(PTRACE_PEEKUSER,
                      child, 8 * ORIG_RAX, NULL);
                      
            if(orig_rax == SYS_write) {
                if(insyscall == 0) {
                    insyscall = 1;
                    //first argument
                    params[0] = ptrace(PTRACE_PEEKUSER, 
                                        child, 8 * RBX, NULL);
                    //second argument
                    params[1] = ptrace(PTRACE_PEEKUSER, 
                                        child, 8 * RCX, NULL);
                    //third argument
                    params[2] = ptrace(PTRACE_PEEKUSER, 
                                        child, 8 * RDX, NULL);

                    str = (char *)malloc((params[2]+1)* sizeof(char));
                    getdata(child, params[1], str,
                            params[2]);
                    reverse(str);
                    putdata(child, params[1], str,
                            params[2]);
                    break;
                } else {
                    orig_rax = ptrace(PTRACE_PEEKUSER, 
                                        child, 8 * RAX, NULL);
                    printf("RAX: %ld\n", orig_rax);
                    insyscall = 0;
                }
            }
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }



}
return 0;
}