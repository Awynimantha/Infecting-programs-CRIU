#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <stdio.h>

int main() {
    pid_t child;
    struct user_regs_struct regs;

    child = fork();
    if (child == 0) {
        // Child process
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    } else if (child > 0) {
        // Parent process
        int status;
        waitpid(child, &status, 0);
        
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            fprintf(stderr, "Child terminated unexpectedly\n");
            return 1;
        }

        // Continue execution of the child process
        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        waitpid(child, &status, 0);
        
        // Read the registers
        ptrace(PTRACE_GETREGS, child, NULL, &regs);
        printf("The child made a system call %ld\n", regs.orig_rax);

        // Continue the child process
        ptrace(PTRACE_CONT, child, NULL, NULL);
        waitpid(child, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
    } else {
        perror("fork");
        return 1;
    }

    return 0;
}
