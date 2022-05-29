/*
 * NAME: EasyFSB
 * GOAL: Pop a shell!
*/

// ubuntu 18.04, gcc 7.5.0
// gcc -o EasyFSB EasyFSB.c -no-pie -z norelro -z execstack -m32

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    system("echo hi");

    for (int i = 0; i < 10; i++)
    {
        char buf[512];
        gets(buf);
        fprintf(stderr, buf);
    }

    return 0;
}