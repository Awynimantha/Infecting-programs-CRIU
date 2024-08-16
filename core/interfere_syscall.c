#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
//ptrace read long size data
int long_size = sizeof(long);
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
void getdata(pid_t child, long addr, char *str, int len){
    // 
}