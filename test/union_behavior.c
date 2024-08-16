#include <stdio.h>

int main(){
    union test {
        long value;
        char chars[100];
    } data;

    const char *str = "hello world";
    strcpy(data.chars, str);
    printf("%dl\n", data.value);
    
    
}