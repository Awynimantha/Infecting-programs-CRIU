#include<stdio.h>
#include<unistd.h>

int main() {
    int res = fork();
    if(res == 0) {
        printf("\nChild process printing");
    } else {
        printf("parent process printing");
        printf("\nFork output : %d", res);

    }
}