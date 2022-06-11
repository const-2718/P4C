/*
 * NAME: OOBR (out of bounds read)
 * GOAL: print password
*/

// ubuntu 18.04, gcc 7.5.0
// gcc -o oobr oobr.c -no-pie -fno-stack-protector -z norelro -z execstack

#include <stdio.h>
#include <stdlib.h>

#ifndef PASSWORD
char *password = "FLAG IS HERE";
#else
char *password = PASSWORD;
#endif

char *messages[4] = {
    "message 1",
    "message 2",
    "message 3",
    "message 4"
};

int main(void)
{
    int index = 0;
    scanf("%d", &index);
    puts(messages[index]);
}