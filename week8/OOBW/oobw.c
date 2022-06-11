/*
 * NAME: OOBW (out of bounds write)
 * GOAL: pop a shell!
*/

// ubuntu 18.04, gcc 7.5.0
// gcc -o oobw oobw.c -no-pie -fno-stack-protector -z norelro -z execstack

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void print_something(int i)
{
    printf("put something %d: ", i);
}

void get_shell(void)
{
    system("/bin/sh");
}

void (*fptr)(int) = print_something;
char buf[256];

int main(void)
{
    while (true)
    {
        char stack_buf[256];
        int idx;

        fptr(1);
        fgets(stack_buf, sizeof(stack_buf), stdin);
        idx = atoi(stack_buf);

        fptr(2);
        fgets(&buf[idx], sizeof(buf), stdin);
    }
}