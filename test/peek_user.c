#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ptrace.h>
#include<unistd.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <stdio.h>

int main() {
    pid_t child;

    child = fork();
    int status;
    printf("pid: %d", child);
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls","ls", NULL);

    } else {
        while(1){
            waitpid(child, &status, 0);
            // orig_eax = ptrace(PTRACE_PEEKUSER,
            //          child, 8 * ORIG_EAX, NULL);
        }
    }
}