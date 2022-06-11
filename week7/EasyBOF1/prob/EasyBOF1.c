/*
 * NAME: EasyBOF1
 * GOAL: print "Pass" to stdout.
*/

// ubuntu 18.04, gcc 7.5.0
// gcc -o EasyBOF1 EasyBOF1.c -no-pie -fno-stack-protector -z norelro -z execstack  

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFSIZE 256

int get_rand(int size, char *out)
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0)
        return -1;

    for (int i = 0; i < size; i++)
    {
        unsigned char c;

        if (read(fd, &c, 1) != 1)
            return -1;

        out[i] = (c % 0x60) + 0x20;
    }

    close(fd);

    return 0;
}

int main(void)
{
    char sRand[BUFSIZE];
    char sPass[BUFSIZE];

    if (get_rand(BUFSIZE - 1, sRand) == -1)
        return -1;

    sRand[BUFSIZE - 1] = '\0';

    gets(sPass);

    printf("sRand = %s\n", sRand);
    printf("sPass = %s\n", sPass);

    if (!strcmp(sRand, sPass))
        puts("Pass");
    else
        puts("Fail");

    return 0;
}