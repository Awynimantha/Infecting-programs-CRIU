#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ptrace.h>
#include<unistd.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <stdio.h>
#include<sys/syscall.h>

int main() {
    pid_t child;

    int status;
    struct user_regs_struct regs;
    long orig_rax;
    int insyscall = 0;
    long params[3];
    child = fork();

    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls","ls", NULL);

    } else {
        while(1){
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

                    printf("Write called with " "%ld, %ld, %ld\n",
                            params[0], params[1],params[2]);
                    break;
                } else {
                    orig_rax = ptrace(PTRACE_PEEKUSER, 
                                        child, 8 * RAX, NULL);
                    printf("RAX: %dl\n", orig_rax);
                    insyscall = 0;
                }
            }
            ptrace(PTRACE_SYSCALL,
                   child, NULL, NULL);
        }
    }
    return 0;
}